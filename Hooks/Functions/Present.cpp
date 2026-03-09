#include "../Hooks.h"
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>

DWORD dwOld_D3DRS_COLORWRITEENABLE;

void SaveState(IDirect3DDevice9* device)
{
	device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
}

void RestoreState(IDirect3DDevice9* device) // not restoring everything. Because its not needed.
{
	device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
}

long __stdcall Hooks::Present(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region)
{
	if (!Menu::setup)
		Menu::SetupMenu(device);

	SaveState(device);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	{
		//Render Shit
		Menu::Render();

	}
	ImGui::EndFrame();
	ImGui::Render();

	if (device->BeginScene() == D3D_OK) {
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	RestoreState(device);

	return Hooks::PresentOfunc(device, source_rect, dest_rect, dest_window_override, dirty_region);
}
