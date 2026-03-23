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
	ImGui::Checkbox("EdgeJump", &Config::Movements::bEdgeJump);
	if (Config::Movements::bEdgeJump)
	{
		ImGui::SameLine(); ImGui::Hotkey(&Config::Movements::kEdgeJump, ImVec2(150, 20));
	}

	ImGui::Checkbox("LongJump", &Config::Movements::bLongJump);
	if (Config::Movements::bLongJump)
	{
		ImGui::SameLine(); ImGui::Hotkey(&Config::Movements::kLongJump, ImVec2(150, 20));
	}

	ImGui::Checkbox("MiniJump", &Config::Movements::bMiniJump);
	if (Config::Movements::bMiniJump)
	{
		ImGui::SameLine(); ImGui::Hotkey(&Config::Movements::kMiniJump, ImVec2(150, 20));
	}

	ImGui::Checkbox("EdgeBug", &Config::Movements::bEdgeBug);
	if (Config::Movements::bEdgeBug) 
	{
		ImGui::SameLine(); ImGui::Hotkey(&Config::Movements::kEdgeBug, ImVec2(150, 20));
		ImGui::Text("EdgeBug Ticks");
		ImGui::SliderInt("##EdgeBugTicks", &Config::Movements::EdgeBugTicks, 0, 128);
		ImGui::Checkbox("Advanced EdgeBug Detection", &Config::Movements::ExtendedEdgeBugPaths);
		if (Config::Movements::ExtendedEdgeBugPaths)
		{
			ImGui::Text("EdgeBug Paths");
			ImGui::SliderInt("##EdgeBugPaths", &Config::Movements::EdgeBugPaths, 0, 30);
			ImGui::Text("EdgeBug Angle");
			ImGui::SliderFloat("##EdgeBugAngle", &Config::Movements::EdgeBugAngle, 0.f, 180.f);
		}
	}

	ImGui::Checkbox("Features only for LAN", &Config::Movements::bLAN);
	if(Config::Movements::bLAN)
	{
		ImGui::Checkbox("Reset FallDamage after EdgeBug(Like CS:GO)", &Config::Movements::EdgeBugNoDamage);
		ImGui::Checkbox("Counter-Strike GameMovement", &Config::Movements::bCSGameMovement);
		ImGui::Text("AirAccelerate");
		ImGui::SliderFloat("##AirAccelerate", &Config::Movements::fAiraccelerate, 0.f, 100.f);
	}

	ImGui::Separator();
	ImGui::Text("Visuals");

	ImGui::Checkbox("Enable Weather", &Config::Visuals::bWeather);
	if (Config::Visuals::bWeather)
	{
		ImGui::Text("What type");
		const char* weathertype[] = { "SnowFall", "Rain", "Ash", "Heavy Rain", "Bugs", "Smoke" };
		ImGui::Combo("##weathertype", &Config::Visuals::iWeather, weathertype, IM_ARRAYSIZE(weathertype));
	}

	ImGui::Separator();
	ImGui::Text("Misc");
	ImGui::Checkbox("Insecure Bypass", &Config::Misc::bInsecure);
	ImGui::Text("TickRate");
	ImGui::SliderInt("##TickRate", &Config::Misc::iTickRate, 0, 128);

	ImGui::End();
}