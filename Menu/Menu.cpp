#include "Menu.h"


void Menu::Render() noexcept
{
	ImGuiIO& io = ImGui::GetIO();

	if (!Menu::show_overlay)
	{
		io.MouseDrawCursor = false;
		return;
	}

	io.MouseDrawCursor = true;

	ImGui::SetNextWindowSize(ImVec2(1200, 600));

	ImGui::Begin("Astolfo hook", &show_overlay);

	ImGui::Checkbox("Bhop", &Config::Movements::bBhop);

	ImGui::End();
}