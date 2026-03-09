#pragma once

#include "IClientEntity.h"
#include "../NetVars/NetVars.h"
#include "Const.h"

class C_BaseEntity : public IClientEntity, public IClientModelRenderable
{
public:

};

class C_BasePlayer : public C_BaseEntity
{
public:
	NETVAR("DT_BasePlayer", "m_iHealth", m_iHealth, int)
	NETVAR("DT_BasePlayer", "m_fFlags", m_fFlags, int)
	OFFSET(int, m_MoveType, 0x144)

	bool IsAlive()
	{
		if (!this) return false;
		return this->m_iHealth() > 0;
	}
};