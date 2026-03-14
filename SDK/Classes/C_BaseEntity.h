#pragma once

#include "BaseTypes.h"
#include "Const.h"
#include "IClientEntity.h"
#include "CUserCmd.h"

#include "../NetVars/NetVars.h"
#include "../../Utils/Memory.h"

enum PlayerPhysFlag_e
{
	PFLAG_DIROVERRIDE = (1 << 0),		// override the player's directional control (trains, physics gun, etc.)
	PFLAG_DUCKING = (1 << 1),		// In the process of ducking, but totally squatted yet
	PFLAG_USING = (1 << 2),		// Using a continuous entity
	PFLAG_OBSERVER = (1 << 3),		// player is locked in stationary cam mode. Spectators can move, observers can't.
	PFLAG_VPHYSICS_MOTIONCONTROLLER = (1 << 4),	// player is physically attached to a motion controller
	PFLAG_GAMEPHYSICS_ROTPUSH = (1 << 5), // game physics did a rotating push that we may want to override with vphysics

	// If you add another flag here check that you aren't 
	// overwriting phys flags in the HL2 of TF2 player classes
};

// Spectator Movement modes
enum
{
	OBS_MODE_NONE = 0,	// not in spectator mode
	OBS_MODE_DEATHCAM,	// special mode for death cam animation
	OBS_MODE_FREEZECAM,	// zooms to a target, and freeze-frames on them
	OBS_MODE_FIXED,		// view from a fixed camera position
	OBS_MODE_IN_EYE,	// follow a player in first person view
	OBS_MODE_CHASE,		// follow a player in third person view
	OBS_MODE_ROAMING,	// free roaming

	NUM_OBSERVER_MODES,
};

// types of precipitation
enum precipitation_type_t {
	PRECIPITATION_TYPE_RAIN = 0,
	PRECIPITATION_TYPE_SNOW,
	PRECIPITATION_TYPE_ASH,
	PRECIPITATION_TYPE_SNOWFALL,
	PRECIPITATION_TYPE_PARTICLERAIN,
	PRECIPITATION_TYPE_PARTICLEASH,
	PRECIPITATION_TYPE_PARTICLERAINSTORM,
	PRECIPITATION_TYPE_PARTICLESNOW,
	NUM_PRECIPITATION_TYPES
};

class ICollideable
{
public:
	// Gets at the entity handle associated with the collideable
	virtual IHandleEntity* GetEntityHandle() = 0;

	// These methods return the bounds of an OBB measured in "collision" space
	// which can be retreived through the CollisionToWorldTransform or
	// GetCollisionOrigin/GetCollisionAngles methods
	virtual Vector& OBBMins() = 0;
	virtual Vector& OBBMaxs() = 0;
};

class CBaseEntity : public IClientEntity, public IClientModelRenderable
{
public:

};

class CBasePlayer : public CBaseEntity
{
public:
	NETVAR("DT_BasePlayer", "m_iHealth", m_iHealth, int)
	NETVAR("DT_BasePlayer", "m_fFlags", m_fFlags, int)
	NETVAR("DT_BasePlayer", "m_vecOrigin", m_vecOrigin, Vector)
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", m_vecVelocity, Vector)
	NETVAR("DT_BasePlayer", "m_flFallVelocity", m_flFallVelocity, float)
	NETVAR("DT_BasePlayer", "m_nTickBase", m_nTickBase, int)
	NETVAR("DT_BasePlayer", "m_flStepSize", m_flStepSize, float)
	NETVAR("DT_BasePlayer", "m_iObserverMode", m_iObserverMode, int)
	NETVAR("DT_BasePlayer", "m_vecViewOffset", m_vecViewOffset, Vector)
	NETVAR_PTR("DT_BasePlayer", "m_hGroundEntity", m_hGroundEntity, CBaseEntity);

	OFFSET(int, m_MoveType, 0x144)
	OFFSET(int, m_afButtonDisabled, 0x1420)
	OFFSET(int, m_afButtonForced, 0x1424)
	OFFSET(float, m_surfaceFriction, 0x13C4)
	OFFSET(float, m_flFallVelocity_server, 0x1AA8)
	OFFSET(unsigned int, m_afPhysicsFlags, 0x1D24)
	datamap_t* DataMap() {
		return memory::Call<datamap_t*>(this, 18);
	}

	FINDPDATAMAP(DataMap(), "m_nButtons", m_nButtons, int)
	FINDDATAMAP(DataMap(), "m_afButtonLast", m_afButtonLast, int)
	FINDDATAMAP(DataMap(), "m_afButtonPressed", m_afButtonPressed, int)
	FINDDATAMAP(DataMap(), "m_afButtonReleased", m_afButtonReleased, int)
	FINDDATAMAP(DataMap(), "m_nImpulse", m_nImpulse, byte)

	ICollideable* Collideable() {
		using original_fn = ICollideable * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}

	bool IsAlive()
	{
		if (!this) return false;
		return this->m_iHealth() > 0;
	}

	bool IsObserver()	
	{
		return (m_afPhysicsFlags() & PFLAG_OBSERVER) != 0;
	}

	CUserCmd** m_pCurrentCommand() {
		auto offset = netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hViewEntity")) - 0x4;
		return reinterpret_cast<CUserCmd**>(uintptr_t(this) + offset);
	}

	CBasePlayer* UTIL_PlayerByIndex(int index)
	{
		using fn = CBasePlayer*(__cdecl*)(int);
		static fn UTIL_PlayerByIndexfn = (fn)memory::PatternScan("server.dll", "55 8B EC 8B 45 08 57 33 FF 85 C0 7E 4E 8B 0D");
		return UTIL_PlayerByIndexfn(index);
	}

	void AddFlag(int flags)
	{
		this->m_fFlags() |= flags;
	}

	void RemoveFlag(int flags)
	{
		this->m_fFlags() &= ~flags;
	}

	void SetViewOffset(const Vector& v)
	{
		this->m_vecViewOffset() = v;
	}
};

class CPlayer_Server : public CBaseEntity
{
public:
	NETVAR("DT_BasePlayer", "m_iHealth", m_iHealth, int)
	NETVAR("DT_BasePlayer", "m_fFlags", m_fFlags, int)
	NETVAR("DT_BasePlayer", "m_vecOrigin", m_vecOrigin, Vector)
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", m_vecVelocity, Vector)
	NETVAR("DT_BasePlayer", "m_flFallVelocity", m_flFallVelocity, float)
	NETVAR("DT_BasePlayer", "m_nTickBase", m_nTickBase, int)
	NETVAR("DT_BasePlayer", "m_flStepSize", m_flStepSize, float)
	NETVAR("DT_BasePlayer", "m_iObserverMode", m_iObserverMode, int)
	NETVAR("DT_BasePlayer", "m_vecViewOffset", m_vecViewOffset, Vector)
	NETVAR_PTR("DT_BasePlayer", "m_hGroundEntity", m_hGroundEntity, CBaseEntity);

	OFFSET(int, m_MoveType, 0x144)
	OFFSET(int, m_afButtonDisabled, 0x1420)
	OFFSET(int, m_afButtonForced, 0x1424)
	OFFSET(float, m_surfaceFriction, 0x13C4)
	OFFSET(float, m_flFallVelocity_server, 0x1AA8)
	OFFSET(unsigned int, m_afPhysicsFlags, 0x144)

	datamap_t* DataMap() {
		return memory::Call<datamap_t*>(this, 18);
	}

	FINDPDATAMAP(DataMap(), "m_nButtons", m_nButtons, int)
	FINDDATAMAP(DataMap(), "m_afButtonLast", m_afButtonLast, int)
	FINDDATAMAP(DataMap(), "m_afButtonPressed", m_afButtonPressed, int)
	FINDDATAMAP(DataMap(), "m_afButtonReleased", m_afButtonReleased, int)
	FINDDATAMAP(DataMap(), "m_nImpulse", m_nImpulse, byte)

	ICollideable* Collideable() {
		using original_fn = ICollideable * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}

	bool IsAlive()
	{
		if (!this) return false;
		return this->m_iHealth() > 0;
	}

	bool IsObserver()
	{
		return (m_afPhysicsFlags() & PFLAG_OBSERVER) != 0;
	}

	CUserCmd** m_pCurrentCommand() {
		auto offset = netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hViewEntity")) - 0x4;
		return reinterpret_cast<CUserCmd**>(uintptr_t(this) + offset);
	}

	CBasePlayer* UTIL_PlayerByIndex(int index)
	{
		using fn = CBasePlayer * (__cdecl*)(int);
		static fn UTIL_PlayerByIndexfn = (fn)memory::PatternScan("server.dll", "55 8B EC 8B 45 08 57 33 FF 85 C0 7E 4E 8B 0D");
		return UTIL_PlayerByIndexfn(index);
	}

	void AddFlag(int flags)
	{
		this->m_fFlags() |= flags;
	}

	void RemoveFlag(int flags)
	{
		this->m_fFlags() &= ~flags;
	}

	void SetViewOffset(const Vector& v)
	{
		this->m_vecViewOffset() = v;
	}
};

class CPrecipitation
{
public:
	NETVAR("DT_Precipitation", "m_nPrecipType", m_nPrecipType, precipitation_type_t)
	NETVAR("DT_BaseEntity", "m_nModelIndex", m_nModelIndex, int)
	OFFSET(unsigned int, GetIndex, 0x58)


	ICollideable* Collideable() {
		using original_fn = ICollideable * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}

	void* Networkable() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}

	void OnPreDataChanged(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)Networkable())[3](Networkable(), update_type);
	}
	void OnDataChanged(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)Networkable())[4](Networkable(), update_type);
	}
	void PreDataUpdate(int update_type) {
		using original_fn = void(__fastcall*)(void*, int, int);
		return (*(original_fn**)Networkable())[5](Networkable(), 0, update_type);
	}
	void PostDataUpdate(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)Networkable())[6](Networkable(), update_type);
	}
	void Release(int a2) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)Networkable())[2](Networkable(), a2);
	}

};