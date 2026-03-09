#pragma once
#include <Windows.h>
//SDK
#include "../SDK/Interfaces/IBaseClientDLL.h"
#include "../SDK/Interfaces/IClientEntityList.h"
#include "../SDK/Interfaces/IVEngineClient.h"
#include "../SDK/Interfaces/ISurface.h"
#include "../SDK/Interfaces/IInputSystem.h"
#include "../SDK/Classes/IClientState.h"
#include "../SDK/Classes/CInput.h"

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

	//patterns
	inline IClientState* client_state = nullptr;
	inline CInput* input = nullptr;
}