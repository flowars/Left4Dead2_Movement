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
	}

}