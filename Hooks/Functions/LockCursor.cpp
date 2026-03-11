#include "../Hooks.h"

void __fastcall Hooks::LockCursorHook(ISurface* ecx)
{
	if (Menu::show_overlay)
	{
		interfaces::surface->UnlockCursor();
		interfaces::input_system->EnableInput(false);
		return;
	}

	interfaces::input_system->EnableInput(true);
	Hooks::LockCursorOriginal(ecx);
}

