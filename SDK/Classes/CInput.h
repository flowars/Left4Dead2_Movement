#pragma once
#include "CUserCmd.h"

#define MULTIPLAYER_BACKUP 150

class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	CRC32_t		m_crc;
};

class CInput
{
public:
	char pad_0000[5]; //0x0000
	bool bTrackIRAvailable; //0x0005
	bool bMouseInitialized; //0x0006
	bool bMouseActive; //0x0007
	char pad_0008[169]; //0x0008
	bool m_bInThirdPerson; //0x00B1
	char pad_00B2[38]; //0x00B2
	Vector camera_offset; //0x00D8
	char pad_00E4[8]; //0x00E4
	CUserCmd* m_pCommands; //0x00EC
	CVerifiedUserCmd* m_pVerifiedCommands; //0x00F0

	CUserCmd* GetUserCmd(int sequence_number) {
		return  &this->m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
	}

	CVerifiedUserCmd* GetVerifiedCommands(int sequence_number) {
		return &m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
	}
};