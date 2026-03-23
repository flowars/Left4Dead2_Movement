#include "../Hooks.h"



double Hooks::GetTickInterval()
{
	return 1.f / Config::Misc::iTickRate;
}