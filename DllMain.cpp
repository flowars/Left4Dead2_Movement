#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include "Windows.h"

// !Yey! Beatiful colors for our console output !!!
#include "Utils/Colored_cout.h"
#include "Utils/Console.h"
#include "Core/Globals.h"
#include "Hooks/Hooks.h"
#include "Core/Interfaces.h"

HMODULE HlModule = nullptr;
bool Loop = true;

void Thread()
{

#ifdef _DEBUG
	Console::Attach_Console();
	SetConsoleTitleA("Astolfo hook | [L4D2]");
	std::cout << clr::magenta << "Base Address: 0x" << &Window::Base << std::endl;
#endif

	try
	{
		//Main functions
		Menu::Core();
		interfaces::Setup();
		Hooks::Setup();
	}
	catch (const std::exception& err)
	{
#ifdef _DEBUG
		std::cout << clr::red << "Error: " << err.what() << std::endl;
#endif 
	}
	while (Loop)
	{
		if (GetAsyncKeyState(VK_F1) & 1 && GetAsyncKeyState(VK_F2) & 1) {
			Loop = false;
		}
	}

#ifdef _DEBUG
	Console::Detach_Console();
#endif

	Hooks::Destroy();
	Menu::Destroy();

	FreeLibraryAndExitThread(HlModule, 1);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {

	switch (reason)
	{
	case DLL_PROCESS_ATTACH: {
		HlModule = module;
		Window::Base = (uintptr_t)GetModuleHandleW(nullptr);
		DisableThreadLibraryCalls(module);
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Thread), nullptr, 0, nullptr);
	} break;
	case DLL_PROCESS_DETACH:

		break;
	}
	return TRUE;
}