#include "../Hooks.h"
#include "../../Features/Visuals/Visuals.h"

void* __fastcall Hooks::GetVCollideHook(void* ecx, void* edx, int model_index)
{
	if (model_index == -1)
		return Visuals::GetVCollideble();

	return Hooks::GetVCollideOriginal(ecx, edx, model_index);
}