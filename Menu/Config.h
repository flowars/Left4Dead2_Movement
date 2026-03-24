#pragma once

namespace Config {

	namespace Movements
	{
		inline bool bBhop = false;

		inline bool bEdgeJump = false;
		inline int kEdgeJump;

		inline bool bLongJump = false;
		inline int kLongJump;

		inline bool bMiniJump = false;
		inline int kMiniJump;

		inline bool bEdgeBug = false;
		inline int kEdgeBug;
		inline int EdgeBugTicks = 12;
		inline bool ExtendedEdgeBugPaths = false;
		inline int EdgeBugPaths = 2;
		inline float EdgeBugAngle = 15.f;

		inline bool bLAN = true;
		inline bool bCSGameMovement = false;
		inline bool EdgeBugNoDamage = false;

		inline float fAiraccelerate = 10.f;
	}

	namespace Visuals
	{
		inline bool bWeather = false;
		inline int iWeather = 0;

		inline bool bFog = false;
		inline float fFogDistance = 0.f;
		inline float bFogDensity = 0.f;
		inline float fFogColor[3] = { 0.f, 0.f, 0.f };
	}

	namespace Misc
	{
		inline bool bInsecure = false;
		inline int iTickRate = 30;
	}

}