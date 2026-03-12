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

	if (MH_CreateHook(memory::PatternScan("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 FC 56 57 8B 43 0C 8B F1 8B 0D"), &Hooks::TryPlayerMoveClient,
		reinterpret_cast<void**>(&Hooks::TryPlayerMoveClientOriginal))) throw std::runtime_error("Failed to Hook TryPlayerMove");
	else
		std::cout << clr::green << ("TryPlayerMove Hooked") << std::endl;

	if (MH_CreateHook(memory::PatternScan("server.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 FC 56 57 8B 43 0C 8B F1 8B 0D"), &Hooks::TryPlayerMoveServer,
		reinterpret_cast<void**>(&Hooks::TryPlayerMoveServerOriginal))) throw std::runtime_error("Failed to Hook TryPlayerMove(server)");
	else
		std::cout << clr::green << ("TryPlayerMove(server) Hooked") << std::endl;

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
