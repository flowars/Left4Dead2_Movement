#pragma once
#include "IGamesystem.h"
#include "SharedDefs.h"


class CGameRules
{
public:
	CViewVectors* GetViewVectors()
	{
		return memory::Call<CViewVectors*>(this, 30);
	}
};