#pragma once

#include "BaseTypes.h"
#include "Const.h"
#include "IClientEntity.h"
#include "CUserCmd.h"

#include "../NetVars/NetVars.h"
#include "../../Utils/Memory.h"

class ICollideable
{
public:
	// Gets at the entity handle associated with the collideable
	virtual IHandleEntity* GetEntityHandle() = 0;

	// These methods return the bounds of an OBB measured in "collision" space
	// which can be retreived through the CollisionToWorldTransform or
	// GetCollisionOrigin/GetCollisionAngles methods
	virtual const Vector& OBBMins() const = 0;
	virtual const Vector& OBBMaxs() const = 0;
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
	NETVAR_PTR("DT_BasePlayer", "m_hGroundEntity", m_hGroundEntity, CBaseEntity);

	OFFSET(int, m_MoveType, 0x144)
	OFFSET(int, m_afButtonDisabled, 0x1420)
	OFFSET(int, m_afButtonForced, 0x1424)
	OFFSET(float, m_surfaceFriction, 0x13C4)
	OFFSET(float, m_flFallVelocity_server, 0x1AA8)
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

	//CUserCmd& Last_Command() {
	//	static const unsigned int last_command_fn =
	//		*reinterpret_cast<unsigned int*>(memory::PatternScan("client.dll", "89 BE ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 89 35 ? ? ? ?") + 0x2);
	//	return *reinterpret_cast<CUserCmd*>(reinterpret_cast<unsigned int>(this) + last_command_fn);
	//}
};