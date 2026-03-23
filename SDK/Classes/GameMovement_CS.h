#pragma once
#include "../../Core/Interfaces.h"

// Height above entity position where the viewer's eye is.

#define VEC_VIEW			interfaces::game_rules->GetViewVectors()->m_vView
#define VEC_HULL_MIN		interfaces::game_rules->GetViewVectors()->m_vHullMin
#define VEC_HULL_MAX		interfaces::game_rules->GetViewVectors()->m_vHullMax

#define VEC_DUCK_HULL_MIN	interfaces::game_rules->GetViewVectors()->m_vDuckHullMin
#define VEC_DUCK_HULL_MAX	interfaces::game_rules->GetViewVectors()->m_vDuckHullMax
#define VEC_DUCK_VIEW		interfaces::game_rules->GetViewVectors()->m_vDuckView

#define VEC_OBS_HULL_MIN	interfaces::game_rules->GetViewVectors()->m_vObsHullMin
#define VEC_OBS_HULL_MAX	interfaces::game_rules->GetViewVectors()->m_vObsHullMax

#define VEC_DEAD_VIEWHEIGHT	interfaces::game_rules->GetViewVectors()->m_vDeadViewHeight



// If the player (enemy bots) are scaled, adjust the hull
#define VEC_VIEW_SCALED( player )				( interfaces::game_rules->GetViewVectors()->m_vView * player->m_flModelScale() )
#define VEC_HULL_MIN_SCALED( player )			( interfaces::game_rules->GetViewVectors()->m_vHullMin * player->m_flModelScale() )
#define VEC_HULL_MAX_SCALED( player )			( interfaces::game_rules->GetViewVectors()->m_vHullMax * player->m_flModelScale() )

#define VEC_DUCK_HULL_MIN_SCALED( player )		( interfaces::game_rules->GetViewVectors()->m_vDuckHullMin * player->m_flModelScale() )
#define VEC_DUCK_HULL_MAX_SCALED( player )		( interfaces::game_rules->GetViewVectors()->m_vDuckHullMax * player->m_flModelScale() )
#define VEC_DUCK_VIEW_SCALED( player )			( interfaces::game_rules->GetViewVectors()->m_vDuckView * player->m_flModelScale() )

#define VEC_OBS_HULL_MIN_SCALED( player )		( interfaces::game_rules->GetViewVectors()->m_vObsHullMin * player->m_flModelScale() )
#define VEC_OBS_HULL_MAX_SCALED( player )		( interfaces::game_rules->GetViewVectors()->m_vObsHullMax * player->m_flModelScale() )

#define VEC_DEAD_VIEWHEIGHT_SCALED( player )	( interfaces::game_rules->GetViewVectors()->m_vDeadViewHeight * player->m_flModelScale() )

#define WATERJUMP_HEIGHT			8

#define MAX_CLIMB_SPEED		200

#define TIME_TO_DUCK		0.4
#define TIME_TO_DUCK_MS		400.0f

#define TIME_TO_UNDUCK		0.2
#define TIME_TO_UNDUCK_MS	200.0f

#define MAX_WEAPON_SLOTS		6	// hud item selection slots
#define MAX_WEAPON_POSITIONS	20	// max number of items within a slot
#define MAX_ITEM_TYPES			6	// hud item selection slots
#define MAX_WEAPONS				48	// Max number of weapons available

#define MAX_ITEMS				5	// hard coded item types

#define WEAPON_NOCLIP			-1	// clip sizes set to this tell the weapon it doesn't use a clip

#define	MAX_AMMO_TYPES	32		// ???
#define MAX_AMMO_SLOTS  32		// not really slots

#define HUD_PRINTNOTIFY		1
#define HUD_PRINTCONSOLE	2
#define HUD_PRINTTALK		3
#define HUD_PRINTCENTER		4

#define STAMINA_MAX				100.0
#define STAMINA_COST_JUMP		25.0
#define STAMINA_COST_FALL		20.0
#define STAMINA_RECOVER_RATE	19.0

enum
{
	SPEED_CROPPED_RESET = 0,
	SPEED_CROPPED_DUCK = 1,
	SPEED_CROPPED_WEAPON = 2,
};

const float CS_PLAYER_SPEED_RUN = 260.0f;
const float CS_PLAYER_SPEED_VIP = 227.0f;
const float CS_PLAYER_SPEED_WALK = 100.0f;
const float CS_PLAYER_SPEED_SHIELD = 160.0f;
const float CS_PLAYER_SPEED_STOPPED = 1.0f;
const float CS_PLAYER_SPEED_OBSERVER = 900.0f;

const float CS_PLAYER_SPEED_DUCK_MODIFIER = 0.34f;
const float CS_PLAYER_SPEED_WALK_MODIFIER = 0.52f;
const float CS_PLAYER_SPEED_CLIMB_MODIFIER = 0.34f;

inline float SimpleSpline(float value)
{
	float valueSquared = value * value;

	// Nice little ease-in, ease-out spline-like curve
	return (3 * valueSquared - 2 * valueSquared * value);
}

class CS_GameMovement
{
public:
	void FinishUnDuck(bool is_server = false);
	void FinishDuck(bool is_server = false);
	bool CanUnduck(bool is_server = false);
	void HandleDuckingSpeedCrop(bool is_server = false);
	void UpdateDuckJumpEyeOffset(bool is_server = false);
	void SetBbox();
};

inline CS_GameMovement cs_gamemovement;

inline void UTIL_TraceHull(const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& hullMin,
	const Vector& hullMax, unsigned int mask, CBaseEntity* ignore,
	int collisionGroup, trace_t* ptr, bool is_server);

inline void UTIL_TraceHull(const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& hullMin,
	const Vector& hullMax, unsigned int mask, ITraceFilter* pFilter, trace_t* ptr, bool is_server);