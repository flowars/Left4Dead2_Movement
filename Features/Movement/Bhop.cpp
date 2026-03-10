#include "Movement.h"


void Movement::Bhop()
{
	if (!Config::Movements::bBhop)
		return;

	if (l4d2::local->m_MoveType() == MOVETYPE_LADDER || l4d2::local->m_MoveType() == MOVETYPE_NOCLIP)
		return;

	if (!(l4d2::local->m_fFlags() & FL_ONGROUND))
		l4d2::cmd->buttons &= ~IN_JUMP;
}

void Movement::EdgeJump()
{
	if (!Config::Movements::bEdgeJump)
		return;

	if (l4d2::local->m_MoveType() == MOVETYPE_LADDER || l4d2::local->m_MoveType() == MOVETYPE_NOCLIP)
		return;

	if(CheckKey(Config::Movements::kEdgeJump))
	{
		if (Prediction_backup::flags & FL_ONGROUND && !(l4d2::local->m_fFlags() & FL_ONGROUND))
			l4d2::cmd->buttons |= IN_JUMP;
	}
}

static int saved_tick_count;

void Movement::LongJump()
{
	if (!Config::Movements::bLongJump)
		return;

	if (l4d2::local->m_MoveType() == MOVETYPE_LADDER || l4d2::local->m_MoveType() == MOVETYPE_NOCLIP)
		return;

	if (l4d2::local->m_fFlags() & FL_ONGROUND)
		saved_tick_count = interfaces::globals->tickcount;

	if (interfaces::globals->tickcount - saved_tick_count > 2)
		Should_LJ = false;
	else
		Should_LJ = true;

	if (CheckKey(Config::Movements::kLongJump))
	{
		if (Should_LJ && !(l4d2::local->m_fFlags() & FL_ONGROUND))
		{
			l4d2::cmd->buttons |= IN_DUCK;
		}
	}
}

void Movement::MiniJump()
{
	if (!Config::Movements::bMiniJump)
		return;

	if (l4d2::local->m_MoveType() == MOVETYPE_LADDER || l4d2::local->m_MoveType() == MOVETYPE_NOCLIP)
		return;

	if (CheckKey(Config::Movements::kMiniJump))
	{
		if (Prediction_backup::flags & FL_ONGROUND && !(l4d2::local->m_fFlags() & FL_ONGROUND))
		{
			l4d2::cmd->buttons |= IN_JUMP;
			l4d2::cmd->buttons |= IN_DUCK;
		}
	}
}