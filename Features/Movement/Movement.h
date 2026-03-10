#pragma once
#include "../../Core/Globals.h"
#include "../Menu/Config.h"
#include "../Prediction/Prediction.h"
#include "../../Menu/Menu.h"

namespace Movement
{
	inline bool Should_LJ = false;

	void Bhop();
	void EdgeJump();
	void LongJump();
	void MiniJump();

}