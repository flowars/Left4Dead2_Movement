#include "Interfaces.h"

void interfaces::Setup()
{
	//interfaces
	client = Capture<IBaseClientDLL>("client.dll", "VClient016");
	entity_list = Capture<IClientEntityList>("client.dll", "VClientEntityList003");
	engine = Capture<IVEngineClient>("engine.dll", "VEngineClient013");
	surface = Capture<ISurface>("vguimatsurface.dll", "VGUI_Surface031");
	input_system = Capture<IInputSystem>("inputsystem.dll", "InputSystemVersion001");
	game_movement = Capture<CGameMovement>("client.dll", "GameMovement001");
	prediction = Capture<CPrediction>("client.dll", "VClientPrediction001");
	cvar = Capture<ICvar>("vstdlib.dll", "VEngineCvar007");
	engine_sound = Capture<void>("engine.dll", "IEngineSoundClient003");
	trace = Capture<IEngineTrace>("engine.dll", "EngineTraceClient003");
	panel = Capture<IPanel>("vgui2.dll", "VGUI_Panel009");
	engineVGUI = Capture<IEngineVGui>("engine.dll", "VEngineVGui001");
	physics_collision = Capture<IPhysicsCollision>("vphysics.dll", "VPhysicsCollision007");
	model_info = Capture<IVModelInfo>("engine.dll", "VModelInfoClient004");

	//server
	game_movement_server = Capture<CGameMovementServer>("server.dll", "GameMovement001");
	trace_server = Capture<IEngineTrace>("engine.dll", "IEngineSoundServer003");
	move_helper_server = **reinterpret_cast<IMoveHelper***>(memory::PatternScan("server.dll", "A1 ? ? ? ? 8B 10 8B 52 1C 81 C1 ? ? ? ? 68 ? ? ? ?") + 1);
	game_rules = **reinterpret_cast<CGameRules***>(memory::PatternScan("server.dll", "8B 0D ? ? ? ? 3B CE 74 09 8B 01 8B 50 30 6A 01 FF D2") + 2);

	//paterns
	client_state = **reinterpret_cast<IClientState***>(memory::PatternScan("engine.dll", "A1 ? ? ? ? 83 C0 08 C3") + 1);
	input = **reinterpret_cast<CInput***>(memory::PatternScan("client.dll", "8B 0D ? ? ? ? 8B 11 8B 42 28 68 ? ? ? ?") + 2);
	move_helper = **reinterpret_cast<IMoveHelper***>(memory::PatternScan(("client.dll"), ("A1 ? ? ? ? 8B 10 8B 52 1C 81 C1 ? ? ? ? 68 ? ? ? ?")) + 1);
	globals = **reinterpret_cast<CGlobalVarsBase***>(memory::PatternScan(("client.dll"), ("8B 0D ? ? ? ? 89 35 ? ? ? ? 0F 57 C0 F3 0F 2A 86 ? ? ? ?")) + 2);
	commandline = *reinterpret_cast<void**>(memory::PatternScan(("tier0.dll"), ("B8 ? ? ? ? C3")) + 1);
}

template <typename Interface>
Interface* interfaces::Capture(const char* moduleName, const char* interfaceName) noexcept
{
	const HINSTANCE handle = GetModuleHandle(moduleName);

	if (!handle)
		return nullptr;

	// get the exported Createinterface function
	using CreateInterfaceFn = Interface * (__cdecl*)(const char*, int*);
	const CreateInterfaceFn createInterface =
		reinterpret_cast<CreateInterfaceFn>(GetProcAddress(handle, "CreateInterface"));

	// return the interface pointer by calling the function
	return createInterface(interfaceName, nullptr);
}