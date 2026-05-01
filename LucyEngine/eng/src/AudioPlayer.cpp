#include "AudioPlayer.h"
#include <cassert>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>
#include <map>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>

class eng::AudioPlayer::impl {
public:
	impl() {
		
	};

	~impl() {

	}

	void PlaySound(std::string sound) {

	}

	void StopSound(std::string sound) {
		// TODO
	}

	void StopSound() {
		// TODO
	}
private:

};

eng::AudioPlayer::AudioPlayer() : m_ImplUptr(std::make_unique<impl>()) {}

eng::AudioPlayer::~AudioPlayer() = default;

void eng::AudioPlayer::PlaySound(std::string sound) {
	m_ImplUptr->PlaySound(sound);
}

void eng::AudioPlayer::StopSound(std::string sound) {
	m_ImplUptr->StopSound(sound);
}

void eng::AudioPlayer::StopSound() {
	m_ImplUptr->StopSound();
}


eng::LoggingAudioPlayer::LoggingAudioPlayer() {}

void eng::LoggingAudioPlayer::PlaySound(std::string sound) {
	service::logger.Get().Log("Sound \"" + sound + "\" played");
	m_AudioPlayer.PlaySound(sound);
}

void eng::LoggingAudioPlayer::StopSound(std::string sound) {
	service::logger.Get().Log("Sound \"" + sound + "\" stopped");
	m_AudioPlayer.StopSound(sound);
}

void eng::LoggingAudioPlayer::StopSound() {
	service::logger.Get().Log("All sound stopped");
	m_AudioPlayer.StopSound();
}

