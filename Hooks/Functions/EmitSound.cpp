#include "../Hooks.h"
#include "../Features/Prediction/Prediction.h"

void __fastcall Hooks::EmitSoundHook(void* ecx, void* edx,
	int a2,
	int a3,
	int a4,
	int a5,
	float a6,
	int a7,
	int a8,
	int a9,
	int a10,
	int a11,
	int a12,
	int a13,
	float a14,
	int a15)
{
	if (!Prediction::InPrediction)
		Hooks::EmitSoundOriginal(ecx, edx, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
}