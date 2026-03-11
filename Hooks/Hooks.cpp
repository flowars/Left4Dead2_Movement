#include "Hooks.h"
#include <stdexcept>
#include <intrin.h>
#include <thread>
#include <iostream>

void Hooks::Setup()
{
	if (MH_Initialize())
		throw std::runtime_error("Failed to init MinHook!");


	if (MH_CreateHook(VF(Menu::device, 17), &Hooks::Present,
		reinterpret_cast<void**>(&Hooks::PresentOfunc))) throw std::runtime_error("Unable to hook Present");
	else
		std::cout << clr::green << "Hooked Present\n";

	if (MH_CreateHook(VF(Menu::device, 16), &Hooks::Reset,
		reinterpret_cast<void**>(&Hooks::ResetOrg))) throw std::runtime_error("Unable to hook Reset");
	else
		std::cout << clr::green << "Hooked Reset\n";


	if (MH_CreateHook(memory::Get(interfaces::client, 20),&Hooks::CreateMoveHook,
		reinterpret_cast<void**>(&Hooks::CreateMoveOriginal))) throw std::runtime_error("Failed to Hook CreateMove");
	else
		std::cout << clr::green << ("CreateMove Hooked") << std::endl;

	if (MH_CreateHook(memory::Get(interfaces::surface, 59), &Hooks::LockCursorHook,
		reinterpret_cast<void**>(&Hooks::LockCursorOriginal))) throw std::runtime_error("Failed to Hook LockCursor");
	else
		std::cout << clr::green << ("LockCursor Hooked") << std::endl;

	if (MH_CreateHook(memory::Get(interfaces::engine_sound, 5), &Hooks::EmitSoundHook,
		reinterpret_cast<void**>(&Hooks::EmitSoundOriginal))) throw std::runtime_error("Failed to Hook EmitSound");
	else
		std::cout << clr::green << ("EmitSound Hooked") << std::endl;


	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Hook Functions error");
	else
		std::cout << clr::magenta << "Enabled Hooks!" << std::endl;

	Menu::DestroyDX();
}

void Hooks::Destroy() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}
