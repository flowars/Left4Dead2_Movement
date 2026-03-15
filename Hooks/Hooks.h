#pragma once
#include <d3d9.h>
#include "MinHook/MinHook.h"

#include "../Core/Globals.h"
#include "../Utils/Colored_cout.h"

#include "../Core/Interfaces.h"
#include "../Utils/Memory.h"

#include "../Menu/Config.h"

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

	using EmitSoundfn = void(__fastcall*)(void*, void*, int, int, int, int, float, int, int, int, int, int, int, int, float, int);
	inline EmitSoundfn EmitSoundOriginal;
	void __fastcall EmitSoundHook(void* ecx, void* edx, int a2, int a3, int a4, int a5, float a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, float a14, int a15);

	using PaintTraversefn = void(__thiscall*)(IPanel*, unsigned int, bool, bool);
	inline PaintTraversefn PaintTraverseOriginal;
	void __stdcall PaintTraverseHook(unsigned int panel, bool force_repaint, bool allow_force);

	using FrameStageFn = void(__fastcall*)(void*, void*, ClientFrameStage_t);
	inline FrameStageFn FrameStageOriginal = nullptr;
	void __fastcall FrameStageHook(void* ecx, void* edx, ClientFrameStage_t curStage);

	using GetVCollidefn = void*(__fastcall*)(void*, void*, int);
	inline GetVCollidefn GetVCollideOriginal;
	void* __fastcall GetVCollideHook(void* ecx, void* edx, int model_index);

	using FindParmFn = int(__thiscall*)(void*, const char*);
	inline FindParmFn FindParmOriginal;
	int __stdcall FindParmHook(const char* psz);

	// GameMovement definition of functions

	using ReduceTimersfn = void(__fastcall*)(void*);
	inline ReduceTimersfn ReduceTimersClientOriginal;
	void __fastcall ReduceTimersClient(void* ecx);

	using ReduceTimersfn = void(__fastcall*)(void*);
	inline ReduceTimersfn ReduceTimersServerOriginal;
	void __fastcall ReduceTimersServer(void* ecx);

	using Duckfn = void(__fastcall*)(void*);
	inline Duckfn DuckClientOriginal;
	void __fastcall DuckClientHook(void* ecx);

	using Duckfn = void(__fastcall*)(void*);
	inline Duckfn DuckServerOriginal;
	void __fastcall DuckServerHook(void* ecx);

	using CategorizePositionfn = void(__thiscall*)(void*, bool);
	inline CategorizePositionfn CategorizePositionOriginal;
	void __stdcall CategorizePositionHook(bool a3);

	using TryPlayerMovefn = int(__fastcall*)(void*, void*, Vector*, trace_t*);
	inline TryPlayerMovefn TryPlayerMoveClientOriginal;
	int __fastcall TryPlayerMoveClient(void* ecx, void* ebp, Vector* pFirstDest, trace_t* pFirstTrace);

	using TryPlayerMovefn = int(__fastcall*)(void*, void*, Vector*, trace_t*);
	inline TryPlayerMovefn TryPlayerMoveServerOriginal;
	int __fastcall TryPlayerMoveServer(void* ecx, void* ebp, Vector* pFirstDest, trace_t* pFirstTrace);
}