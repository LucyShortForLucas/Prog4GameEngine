#include "AudioPlayer.h"
#include <cassert>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>
#include <map>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <Services.h>
#include "HashedString.h"

class eng::AudioPlayer::impl {
public:
	impl() {
		auto& logger{ service::logger.Get() };

		// Set up SDL Mixer
		if (MIX_Init())
			logger.LogSuccess("SDL3_Mixer Initialized!");
		else {
			logger.LogError(std::string("Failed to initialize SDL3_MIXER: ") + SDL_GetError());
			return;
		}

		// set up defaults audio device
		m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
		if (m_pMixer != nullptr)
			logger.LogSuccess("Default Playback Device Mixer successfully created!");
		else
			logger.LogError(std::string("Failed to create default playback device mixer: ") + SDL_GetError());

		// Set up thread
		m_AudioThread = std::jthread([&](std::stop_token stopToken) {
			std::unique_lock f_Lock{ m_AudioToPlayMutex };

			while (!stopToken.stop_requested()) {
				// Wait until there's audio to be played
				m_AudioToPlayCv.wait(
					f_Lock, [this, &stopToken]() -> bool {
						return !m_AudioToPlay.empty();
					}
				);

				// Get next audio to play
				const HashedString audioToPlay{ m_AudioToPlay.front().first };
				int trackId{ m_AudioToPlay.front().second };
				m_AudioToPlay.pop();

				// Audio not loaded yet? -> load it first
				if (!m_Audios.contains(audioToPlay.hash)) {

					auto emplacedAudio{
						m_Audios.emplace(
							audioToPlay.hash,
							MIX_LoadAudio(m_pMixer, audioToPlay.text.c_str(), false)
						)
						.first->second  // extract the value from the key/value pair of the returned std::pair<iterator, bool> from emplace
					};

					if (emplacedAudio != nullptr)
						logger.LogSuccess("Successfully created SDL3_Mixer audio \"" + audioToPlay.text + "\"");
					else
						logger.LogError("Could not create SDL3_Mixer audio \"" + audioToPlay.text + "\"");
				}

				// Fetch available Track or create new one if needed
				std::lock_guard<std::mutex> lock{ m_TracksMutex };
				MIX_Track* chosenTrack{};
				for (auto& [id, track] : m_Tracks) {
					if (MIX_TrackPlaying(track))
						continue;
					chosenTrack = track;
					id = trackId;
				}

				if (chosenTrack == nullptr) {
					logger.Log("No available SDL3 audio tracks, adding SDL3 track...");
					auto emplacedTrack{ m_Tracks.emplace_back(trackId, MIX_CreateTrack(m_pMixer)).second };

					if (emplacedTrack != nullptr)
						logger.LogSuccess("Successfully created new SDL3 audio track");
					else
						logger.LogError(std::string("Failed to create new SDL3 audio track: ") + SDL_GetError());
				}

				// Set up audio track with audio
				if (!MIX_SetTrackAudio(chosenTrack, m_Audios.at(audioToPlay.hash)))
					logger.LogError("Failed to load audio \"" + audioToPlay.text + "\" into track"); // We don't log success on every successful play

				// Play Audio Track
				if (!MIX_PlayTrack(chosenTrack, 0))
					logger.LogError("Failed to play audio \"" + audioToPlay.text + "\""); // We don't log success on every successful play
			}

		});
	};

	~impl() {
		m_AudioThread.join();
		MIX_DestroyMixer(m_pMixer);

		for (auto [id, track] : m_Tracks)
			MIX_DestroyTrack(track);

		for (auto [hash, audio] : m_Audios)
			MIX_DestroyAudio(audio);
	}

	int PlaySound(const HashedString& sound) {
		static int nextId{0};
		int id{ nextId++ };
		std::lock_guard<std::mutex> lock{ m_AudioToPlayMutex };
		m_AudioToPlay.emplace(sound, id);
		m_AudioToPlayCv.notify_one();
		return id;
	}

	void StopSound(int trackId) {
		auto& logger{service::logger.Get() };
		std::lock_guard<std::mutex> lock{ m_TracksMutex };

		MIX_Track* trackToStop{ std::ranges::find_if(m_Tracks, [trackId](std::pair<int, MIX_Track*> trackPair) {
			return trackPair.first == trackId;
			})->second
		};

		if (!MIX_StopTrack(trackToStop, 0))
			logger.LogError(std::string("Failed to stop SDL3 audio Track: ") + SDL_GetError());
			
	}

	void StopSound() {
		auto& logger{ service::logger.Get() };
		if(!MIX_StopAllTracks(m_pMixer, 0))
			logger.LogError(std::string("Failed to stop all SDL3 audio Tracks: ") + SDL_GetError());
	}
private:
	MIX_Mixer* m_pMixer{};
	std::map<unsigned int, MIX_Audio*> m_Audios{};
	std::queue<std::pair<HashedString, int>> m_AudioToPlay{};
	std::mutex m_AudioToPlayMutex{};
	std::condition_variable m_AudioToPlayCv{};
	std::jthread m_AudioThread{};
	std::vector<std::pair<int, MIX_Track*>> m_Tracks{};
	std::mutex m_TracksMutex{};
};

eng::AudioPlayer::AudioPlayer() : m_ImplUptr(std::make_unique<impl>()) {}      

eng::AudioPlayer::~AudioPlayer() = default;

void eng::AudioPlayer::PlaySound(const HashedString& sound) {
	m_ImplUptr->PlaySound(sound);
}

void eng::AudioPlayer::StopSound(int sound) {
	m_ImplUptr->StopSound(sound);
}

void eng::AudioPlayer::StopSound() {
	m_ImplUptr->StopSound();
}


eng::LoggingAudioPlayer::LoggingAudioPlayer() {}

void eng::LoggingAudioPlayer::PlaySound(const HashedString& sound) {
	service::logger.Get().Log("Sound \"" + sound.text + "\" played");
	m_AudioPlayer.PlaySound(sound);
}

void eng::LoggingAudioPlayer::StopSound(int sound) {
	service::logger.Get().Log("Sound stopped");
	m_AudioPlayer.StopSound(sound);
}

void eng::LoggingAudioPlayer::StopSound() {
	service::logger.Get().Log("All sound stopped");
	m_AudioPlayer.StopSound();
}

