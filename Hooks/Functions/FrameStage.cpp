#include "../Hooks.h"
#include "../../Features/Visuals/Visuals.h"


void __fastcall Hooks::FrameStageHook(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	if (curStage == FRAME_RENDER_START)
	{
		Visuals::Update_Weather();
	}
	else if (curStage == FRAME_NET_UPDATE_START)
	{

	}
	else if (curStage == FRAME_START)
	{
	}
	else if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
	}


	Hooks::FrameStageOriginal(ecx, edx, curStage);
}