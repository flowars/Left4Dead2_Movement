#pragma once
#include <Windows.h>
//SDK
#include "../SDK/Interfaces/IBaseClientDLL.h"
#include "../SDK/Interfaces/IClientEntityList.h"
#include "../SDK/Interfaces/IVEngineClient.h"
#include "../SDK/Interfaces/ISurface.h"
#include "../SDK/Interfaces/IInputSystem.h"
#include "../SDK/Interfaces/GameMovement.h"
#include "../SDK/Interfaces/CPrediction.h"
#include "../SDK/Interfaces/IConvar.h"
#include "../SDK/Interfaces/CTrace.h"
#include "../SDK/Interfaces/IPanel.h"
#include "../SDK/Interfaces/IEngineVGui.h"
#include "../SDK/Interfaces/IPhysicsCollision.h"
#include "../SDK/Interfaces/IVModelInfo.h"


#include "../SDK/Classes/IClientState.h"
#include "../SDK/Classes/CInput.h"
#include "../SDK/Classes/CGlobalVarsBase.h"
#include "../SDK/Classes/CGameRules.h"

#include "../Utils/Memory.h"

namespace interfaces
{
	void Setup();

	template <typename Interface>
	Interface* Capture(const char* moduleName, const char* interfaceName) noexcept;

	//interfaces
	inline IBaseClientDLL* client = nullptr;
	inline IClientEntityList* entity_list = nullptr;
	inline IVEngineClient* engine = nullptr;
	inline ISurface* surface = nullptr;
	inline IInputSystem* input_system = nullptr;
	inline CGameMovement* game_movement = nullptr;
	inline CPrediction* prediction = nullptr;
	inline CGlobalVarsBase* globals = nullptr;
	inline ICvar* cvar = nullptr;
	inline IEngineTrace* trace = nullptr;
	inline IPanel* panel = nullptr;
	inline IEngineVGui* engineVGUI = nullptr;
	inline IPhysicsCollision* physics_collision = nullptr;
	inline IVModelInfo* model_info = nullptr;
	inline void* engine_sound = nullptr;
	inline void* commandline = nullptr;

	//server interfaces
	inline CGameMovementServer* game_movement_server = nullptr;
	inline IMoveHelper* move_helper_server = nullptr;
	inline IEngineTrace* trace_server = nullptr;
	inline CGameRules* game_rules = nullptr;

	//patterns
	inline IClientState* client_state = nullptr;
	inline CInput* input = nullptr;
	inline IMoveHelper* move_helper = nullptr;
}