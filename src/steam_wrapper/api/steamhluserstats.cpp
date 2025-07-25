#include "steamhluserstats.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

bool SteamHLUserStats::is_initialized = false;
SteamHLUserStats* SteamHLUserStats::_singleton = nullptr;

SteamHLUserStats::SteamHLUserStats() {
	if (SteamHLUserStats::_singleton != nullptr) {
		// TODO=Doesn't actually print anything to the console
		UtilityFunctions::print("Warning: Someone is creating SteamHLUserStats instead of using the singleton");
	}

	SteamHLUserStats::_singleton = this;
}

SteamHLUserStats::~SteamHLUserStats() {
	SteamHLUserStats::_singleton = nullptr;
}

void SteamHLUserStats::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_achievement", "achievement_name"), &SteamHLUserStats::SetAchievement);
	ClassDB::bind_method(D_METHOD("get_achievement", "achievement_name"), &SteamHLUserStats::GetAchievement);
}

SteamHLUserStats* SteamHLUserStats::get_singleton() {
	if (SteamHLUserStats::_singleton == nullptr) {
		SteamHLUserStats::_singleton = memnew(SteamHLUserStats);
	}
	return SteamHLUserStats::_singleton;
}

bool SteamHLUserStats::SetAchievement(const String& name) {
	steamHLUserStats_LOAD_OR_RETURN(false);
	if (steamUserStats->SetAchievement(name.utf8().get_data())) {
		return steamUserStats->StoreStats();
	}
	else {
		return false;
	}
}

bool SteamHLUserStats::GetAchievement(const String& name) {
	steamHLUserStats_LOAD_OR_RETURN(false);
	bool achieved = false;
	if (steamUserStats->GetAchievement(name.utf8().get_data(), &achieved)) {
		return achieved;
	}
	else {
		return false;
	}
}

// TODO = Since RequestUserStats has been removed, is this callback even used anymore?
// Note: Functions that previously branched on this (Get/Set achievement have had this removed)
void SteamHLUserStats::onUserStatsReceived(UserStatsReceived_t* callback) {
	if (callback->m_eResult == k_EResultOK) { // TODO = This returns failure if there are no stats. Does that include when a player has started for the first time?
		SteamHLUserStats::is_initialized = true;
	}
	else if (!SteamHLUserStats::is_initialized) {
		UtilityFunctions::print("Warning: Failed to retrieve Steam user stats.");
	}
}