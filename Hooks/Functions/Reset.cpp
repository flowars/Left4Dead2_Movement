#include "../Hooks.h"
#include <ImGui/imgui_impl_dx9.h>

HRESULT __stdcall Hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = Hooks::ResetOrg(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}