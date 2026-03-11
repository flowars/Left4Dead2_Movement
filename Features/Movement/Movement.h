#pragma once
#include "../../Core/Globals.h"
#include "../Menu/Config.h"
#include "../Prediction/Prediction.h"
#include "../../Menu/Menu.h"
#include "../../Utils/Math.h"

struct EdgeBug_s {
	bool Crouched;
	int DetectTick;
	int EdgebugTick;
	bool Strafing;
	float YawDelta;
	float Forwardmove, Sidemove;
	float StartingYaw;
	int EbLength;
	int Ticks_Left;
};
inline EdgeBug_s EdgeBug_data;

namespace Movement
{
	inline bool Should_LJ = false;
	inline bool Should_EB = false;
	inline bool Detect_EB = false;

	void Bhop();
	void EdgeJump();
	void LongJump();
	void MiniJump();
	void EdgeBug();
	void FixMovement(Vector& angle);
}