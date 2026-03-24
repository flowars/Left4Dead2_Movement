#include "Visuals.h"

void Visuals::Fog()
{
	static auto fog_override = interfaces::cvar->FindVar("fog_override");

	if (!Config::Visuals::bFog) {
		fog_override->SetValue(0);
		return;
	}
	fog_override->SetValue(1);

	static auto fog_start = interfaces::cvar->FindVar("fog_start");
	static auto fog_skybox_start = interfaces::cvar->FindVar("fog_startskybox");

	fog_start->SetValue(0);
	fog_skybox_start->SetValue(0);

	static auto fog_end = interfaces::cvar->FindVar("fog_end");
	static auto fog_skybox_end = interfaces::cvar->FindVar("fog_endskybox");

	fog_end->SetValue(Config::Visuals::fFogDistance);
	fog_skybox_end->SetValue(Config::Visuals::fFogDistance);

	static auto fog_maxdensity = interfaces::cvar->FindVar("fog_maxdensity");
	static auto fog_skybox_maxdensity = interfaces::cvar->FindVar("fog_maxdensityskybox");

	fog_maxdensity->SetValue((float)Config::Visuals::bFogDensity * 0.01f);
	fog_skybox_maxdensity->SetValue((float)Config::Visuals::bFogDensity * 0.01f);

	int red = Config::Visuals::fFogColor[0] * 255;
	int green = Config::Visuals::fFogColor[1] * 255;
	int blue = Config::Visuals::fFogColor[2] * 255;
	char buffer_color[12];
	sprintf_s(buffer_color, 12, "%i %i %i", red, green, blue);

	static auto fog_color = interfaces::cvar->FindVar("fog_color");
	static auto fog_skybox_color = interfaces::cvar->FindVar("fog_colorskybox");

	fog_color->SetValue(buffer_color);
	fog_skybox_color->SetValue(buffer_color);
}