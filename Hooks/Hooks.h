#pragma once
#include <d3d9.h>
#include "MinHook/MinHook.h"

#include "../Core/Globals.h"
#include "../Utils/Colored_cout.h"

#include "../Core/Interfaces.h"
#include "../Utils/Memory.h"

namespace Hooks
{
	void Setup();
	void Destroy() noexcept;

	constexpr void* VF(void* ptr, size_t index) noexcept
	{
		return (*static_cast<void***>(ptr))[index];
	}

	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOrg = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;

	using Presentfn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
	inline Presentfn PresentOfunc;
	long __stdcall Present(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region);

	using CreateMovefn = void(__fastcall*)(void*, void*, int, float, bool);
	inline CreateMovefn CreateMoveOriginal;
	void __stdcall CreateMoveHook(int sequence_number, float input_sample_frametime, bool active);

	using LockCursorfn = void(__fastcall*)(ISurface*);
	inline LockCursorfn LockCursorOriginal;
	void __fastcall LockCursorHook(ISurface* ecx);
}