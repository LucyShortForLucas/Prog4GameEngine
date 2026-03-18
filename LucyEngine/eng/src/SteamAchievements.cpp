#include "SteamAchievements.h"
#include <cstddef>
#include <steam_api.h>

#pragma warning(push)
#pragma warning(disable : 4081 4389 4996) 

CSteamAchievements::CSteamAchievements(Achievement_t* Achievements, int NumAchievements) :
    m_iAppID(0),
    m_bInitialized(false),
    m_CallbackUserStatsStored(this, &CSteamAchievements::OnUserStatsStored),
    m_CallbackAchievementStored(this, &CSteamAchievements::OnAchievementStored) {
    m_iAppID = SteamUtils()->GetAppID();
    m_pAchievements = Achievements;
    m_iNumAchievements = NumAchievements;
    m_bInitialized = Initialize();
}

bool CSteamAchievements::Initialize() {
	// Is Steam loaded? If not we can't get stats.
	if (NULL == SteamUserStats() || NULL == SteamUser()) {
		return false;
	}
	// Is the user logged on?  If not we can't access stats.
	if (!SteamUser()->BLoggedOn()) {
		return false;
	}

	return true;
}

bool CSteamAchievements::SetAchievement(const char* ID) {
	// Have we received a call back from Steam yet?
	if (m_bInitialized) {
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	// If not then we can't set achievements yet
	return false;
}

void CSteamAchievements::OnUserStatsStored(UserStatsStored_t* pCallback) {
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == pCallback->m_nGameID) {
		if (k_EResultOK == pCallback->m_eResult) {
		}
		else {
			char buffer[128];
			_snprintf(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
		}
	}
}

void CSteamAchievements::OnAchievementStored(UserAchievementStored_t* pCallback) {
	 // we may get callbacks for other games' stats arriving, ignore them
	if (m_iAppID == pCallback->m_nGameID) {
	}
}

CSteamAchievements* g_SteamAchievements = NULL;

Achievement_t g_Achievements[] =
{
	_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
	_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
	_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
	_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
};

#pragma warning(pop)