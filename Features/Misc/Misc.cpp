#include "Misc.h"


void Misc::Cvars()
{
	static ConVar* sv_airaccelerate = interfaces::cvar->FindVar("sv_airaccelerate");

	sv_airaccelerate->SetValue(Config::Movements::fAiraccelerate);
	sv_airaccelerate->m_fnChangeCallbacks.RemoveAll();
}