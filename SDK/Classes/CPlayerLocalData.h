#pragma once
#include "Const.h"

class CPlayerLocalData
{
public:
	char pad_0000[64]; //0x0000
	int32_t m_nOldButtons; //0x0040
	char pad_0044[8]; //0x0044
	int32_t m_flDucktime; //0x004C
	char pad_0050[8]; //0x0050
	float m_flFallVelocity; //0x0058
	float m_flStepSize; //0x005C
	float m_flShootTime; //0x0060
	char pad_0064[4]; //0x0064
	float m_flDuckJumpTime; //0x0068
	char pad_006C[12]; //0x006C
	bool m_bDucked; //0x0078
	bool m_bDucking; //0x0079
	char pad_007C[2]; //0x007C
	bool m_bAllowAutoMovement; //0x007E
};

class CPlayerLocalDataServer
{
public:
	char pad_0000[80]; //0x0000
	bool m_bDucked; //0x0050
	bool m_bDucking; //0x0051
	char pad_0052[2]; //0x0052
	int32_t m_flDucktime; //0x0054
	char pad_0058[12]; //0x0058
	float m_flFallVelocity; //0x0064
	int32_t m_nOldButtons; //0x0068
	char pad_006C[4]; //0x006C
	float m_flShootTime; //0x0070
	char pad_0074[4]; //0x0074
	float m_flDuckJumpTime; //0x0078
	char pad_007C[12]; //0x007C
	bool m_bAllowAutoMovement; //0x0088
	char pad_0089[3]; //0x0089
	float m_flStepSize; //0x008C
	char pad_0090[176]; //0x0090
};