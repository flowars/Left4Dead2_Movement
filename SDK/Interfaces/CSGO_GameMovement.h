#pragma once
#include "../Classes/C_BaseEntity.h"
#include "../../Utils/Memory.h"
#include "CTrace.h"

//CSGO REALISATION
class CViewVectors
{
public:
	CViewVectors() {}

	CViewVectors(
		Vector vView,
		Vector vHullMin,
		Vector vHullMax,
		Vector vDuckHullMin,
		Vector vDuckHullMax,
		Vector vDuckView,
		Vector vObsHullMin,
		Vector vObsHullMax,
		Vector vDeadViewHeight)
	{
		m_vView = vView;
		m_vHullMin = vHullMin;
		m_vHullMax = vHullMax;
		m_vDuckHullMin = vDuckHullMin;
		m_vDuckHullMax = vDuckHullMax;
		m_vDuckView = vDuckView;
		m_vObsHullMin = vObsHullMin;
		m_vObsHullMax = vObsHullMax;
		m_vDeadViewHeight = vDeadViewHeight;
	}

	// Height above entity position where the viewer's eye is.
	Vector m_vView;

	Vector m_vHullMin;
	Vector m_vHullMax;

	Vector m_vDuckHullMin;
	Vector m_vDuckHullMax;
	Vector m_vDuckView;

	Vector m_vObsHullMin;
	Vector m_vObsHullMax;

	Vector m_vDeadViewHeight;
};

static CViewVectors g_CSViewVectors(
	Vector(0, 0, 64),		// eye position

	Vector(-16, -16, 0),	// hull min
	Vector(16, 16, 71),	// hull max

	Vector(-16, -16, 0),	// duck hull min
	Vector(16, 16, 54),	// duck hull max
	Vector(0, 0, 46),		// duck view

	Vector(-10, -10, -10),	// observer hull min
	Vector(10, 10, 10),	// observer hull max

	Vector(0, 0, 14)		// dead view height
);

// Height above entity position where the viewer's eye is.
#define VEC_VIEW			g_CSViewVectors.m_vView
#define VEC_HULL_MIN		g_CSViewVectors.m_vHullMin
#define VEC_HULL_MAX		g_CSViewVectors.m_vHullMax

#define VEC_DUCK_HULL_MIN	g_CSViewVectors.m_vDuckHullMin
#define VEC_DUCK_HULL_MAX	g_CSViewVectors.m_vDuckHullMax
#define VEC_DUCK_VIEW		g_CSViewVectors.m_vDuckView

const float CS_PLAYER_SPEED_RUN = 260.0f;
const float CS_PLAYER_SPEED_VIP = 227.0f;
const float CS_PLAYER_SPEED_SHIELD = 160.0f;
const float CS_PLAYER_SPEED_HAS_HOSTAGE = 200.0f;
const float CS_PLAYER_SPEED_STOPPED = 1.0f;
const float CS_PLAYER_SPEED_OBSERVER = 900.0f;

const float CS_PLAYER_SPEED_DUCK_MODIFIER = 0.34f;
const float CS_PLAYER_SPEED_WALK_MODIFIER = 0.52f;
const float CS_PLAYER_SPEED_CLIMB_MODIFIER = 0.34f;
const float CS_PLAYER_HEAVYARMOR_FLINCH_MODIFIER = 0.5f;

const float CS_PLAYER_DUCK_SPEED_IDEAL = 8.0f;

enum
{
	SPEED_CROPPED_RESET = 0,
	SPEED_CROPPED_DUCK = 1,
	SPEED_CROPPED_WEAPON = 2,
};


//Client
namespace client
{
	static float m_flDuckAmount = 0.f;
	static bool m_bDucked = false;
	static bool m_bDucking = false;
	static int m_nDuckTimeMsecs = 0;
	static Vector2D m_vecLastPositionAtFullCrouchSpeed;
	static float m_flDuckSpeed = 0.f;
	static bool m_bDuckOverride = false;
	static float m_flLastDuckTime = 0.f;
	static float m_flStamina = 0.f;
	static bool m_bAllowAutoMovement = false;
	static bool m_duckUntilOnGround = false;
}
//Server
namespace server
{
	static float m_flDuckAmount = 0.f;
	static bool m_bDucked = false;
	static bool m_bDucking = false;
	static int m_nDuckTimeMsecs = 0;
	static Vector2D m_vecLastPositionAtFullCrouchSpeed;
	static float m_flDuckSpeed = 0.f;
	static bool m_bDuckOverride = false;
	static float m_flLastDuckTime = 0.f;
	static float m_flStamina = 0.f;
	static bool m_bAllowAutoMovement = false;
	static bool m_duckUntilOnGround = false;
}

class CCSGameMovement
{
public:
	const Vector& GetPlayerViewOffset(bool ducked);
	void FinishUnDuck(bool is_server = false);
	bool CanUnduck(bool is_server = false);
	void FinishDuck(bool is_server = false);
	void HandleDuckingSpeedCrop(float duckFraction, bool is_server = false);
	float GetDuckSpeedModifier(float duckFraction) const { return CS_PLAYER_SPEED_DUCK_MODIFIER * duckFraction + 1.0f - duckFraction; }
	void FixPlayerCrouchStuck(bool upward, bool is_server = false);
	CBaseHandle TestPlayerPosition(const Vector& pos, int collisionGroup, trace_t& pm, bool is_server);
};
inline CCSGameMovement g_GameMovement_client;
inline CCSGameMovement g_GameMovement_server;


inline void UTIL_TraceHull(const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& hullMin, const Vector& hullMax, unsigned int mask, CBaseEntity* ignore, int collisionGroup, trace_t* ptr);
inline void UTIL_TraceRay(const Ray_t& ray, unsigned int mask, CBaseEntity* ignore, int collisionGroup, trace_t* ptr);