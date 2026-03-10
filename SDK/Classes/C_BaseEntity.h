#pragma once

#include "BaseTypes.h"
#include "Const.h"
#include "IClientEntity.h"
#include "CUserCmd.h"

#include "../NetVars/NetVars.h"
#include "../../Utils/Memory.h"

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
	NETVAR("DT_BasePlayer", "m_vecVelocity", m_vecVelocity, Vector)
	NETVAR("DT_BasePlayer", "m_nTickBase", m_nTickBase, int)

	OFFSET(int, m_MoveType, 0x144)
	OFFSET(int, m_afButtonDisabled, 0x1420)
	OFFSET(int, m_afButtonForced, 0x1424)

	datamap_t* DataMap() {
		return memory::Call<datamap_t*>(this, 18);
	}

	FINDPDATAMAP(DataMap(), "m_nButtons", m_nButtons, int)
	FINDDATAMAP(DataMap(), "m_afButtonLast", m_afButtonLast, int)
	FINDDATAMAP(DataMap(), "m_afButtonPressed", m_afButtonPressed, int)
	FINDDATAMAP(DataMap(), "m_afButtonReleased", m_afButtonReleased, int)
	FINDDATAMAP(DataMap(), "m_nImpulse", m_nImpulse, byte)

	bool IsAlive()
	{
		if (!this) return false;
		return this->m_iHealth() > 0;
	}

	CUserCmd** m_pCurrentCommand() {
		auto offset = netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hViewEntity")) - 0x4;
		return reinterpret_cast<CUserCmd**>(uintptr_t(this) + offset);
	}

	//CUserCmd& Last_Command() {
	//	static const unsigned int last_command_fn =
	//		*reinterpret_cast<unsigned int*>(memory::PatternScan("client.dll", "89 BE ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 89 35 ? ? ? ?") + 0x2);
	//	return *reinterpret_cast<CUserCmd*>(reinterpret_cast<unsigned int>(this) + last_command_fn);
	//}
};