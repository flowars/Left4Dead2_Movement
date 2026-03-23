#pragma once
#include "../../Utils/Memory.h"

class CServerGameDLL
{
public:
	float GetTickInterval()
	{
		return memory::Call<float>(this, 9);
	}
};