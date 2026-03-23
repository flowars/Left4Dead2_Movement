#include "Misc.h"


void Misc::Cvars()
{
	static ConVar* sv_airaccelerate = interfaces::cvar->FindVar("sv_airaccelerate");

	sv_airaccelerate->SetValue(Config::Movements::fAiraccelerate);
	sv_airaccelerate->m_fnChangeCallbacks.RemoveAll();
}

void Misc::TickRate()
{
	static int i = 30;
	if (Config::Misc::iTickRate != i)
	{
		static ConVar* cl_cmdrate = interfaces::cvar->FindVar("cl_cmdrate");
		static ConVar* cl_updaterate = interfaces::cvar->FindVar("cl_updaterate");
		static ConVar* rate = interfaces::cvar->FindVar("rate");

		cl_cmdrate->SetValue(Config::Misc::iTickRate);
		cl_updaterate->SetValue(Config::Misc::iTickRate);
		rate->SetValue(Config::Misc::iTickRate * 1000);

		/* don't know, but it's change tickrate on server immediatly lol, maybe it's wrong way, and we need immediatle change value in the Hooks::GetTickInterval() when start the game, but it works! */
		static float* tick_interval = *reinterpret_cast<float**>((memory::PatternScan("engine.dll", "D9 15 ? ? ? ? DD 05 ? ? ? ? DB F1 DD 05 ? ? ? ? 77 08")) + 2);
		*tick_interval = 1.f / Config::Misc::iTickRate;
	}
	i = Config::Misc::iTickRate;
}
