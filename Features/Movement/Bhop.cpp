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