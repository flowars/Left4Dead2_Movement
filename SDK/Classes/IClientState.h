#pragma once
#include "BaseTypes.h"
#include "Vector.h"

class IClientState
{
public:
	char pad_0000[24]; //0x0000
	void* m_NetChannel; //0x0018
	char pad_001C[84]; //0x001C
	int m_nSignonState; //0x0070
	char pad_0074[4]; //0x0074
	double m_flNextCmdTime; //0x0078
	int m_nServerCount; //0x0080
	int m_nCurrentSequence; //0x0084
	char pad_0088[76]; //0x0088
	int m_nDeltaTick; //0x00D4
	bool m_bPaused; //0x00D8
	char pad_00D9[15]; //0x00D9
	char m_szLevelNameShort[40]; //0x00E8
	char m_szMapGroupName[40]; //0x0110
	char m_szLastLevelNameShort[40]; //0x0138
	int m_nMaxClients; //0x0160
	int m_nNumPlayersToConnect; //0x0164
	char pad_0168[18640]; //0x0168
	float m_flLastServerTickTime; //0x4A38
	bool insimulation; //0x4A3C
	char pad_4A3D[3]; //0x4A3D
	int oldtickcount; //0x4A40
	float m_tickRemainder; //0x4A44
	float m_frameTime; //0x4A48
	int lastoutgoingcommand; //0x4A4C
	int chokedcommands; //0x4A50
	int last_command_ack; //0x4A54
	int last_server_tick; //0x4A58
	int command_ack; //0x4A5C
	char pad_4A60[76]; //0x4A60
	Vector viewangles; //0x4AAC
};