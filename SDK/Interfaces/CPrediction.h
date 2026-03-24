#pragma once
#include <winnt.h>
#include "../Classes/C_BaseEntity.h"
#include "../Classes/CUserCmd.h"
#include "GameMovement.h"

class IPrediction
{
public:
	virtual			~IPrediction(void) {};

	virtual void	Init(void) = 0;
	virtual void	Shutdown(void) = 0;

	// Run prediction
	virtual void	Update
	(
		int startframe,				// World update ( un-modded ) most recently received
		bool validframe,			// Is frame data valid
		int incoming_acknowledged,	// Last command acknowledged to have been run by server (un-modded)
		int outgoing_command		// Last command (most recent) sent to server (un-modded)
	) = 0;

	// We are about to get a network update from the server.  We know the update #, so we can pull any
	//  data purely predicted on the client side and transfer it to the new from data state.
	virtual void	PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void	PostEntityPacketReceived(void) = 0;
	virtual void	PostNetworkDataReceived(int commands_acknowledged) = 0;

	virtual void	OnReceivedUncompressedPacket(void) = 0;

	// The engine needs to be able to access a few predicted values
	virtual void	GetViewOrigin(Vector& org) = 0;
	virtual void	SetViewOrigin(Vector& org) = 0;
	virtual void	GetViewAngles(Vector& ang) = 0;
	virtual void	SetViewAngles(Vector& ang) = 0;
	virtual void	GetLocalViewAngles(Vector& ang) = 0;
	virtual void	SetLocalViewAngles(Vector& ang) = 0;
};

class CPrediction
{
public:
	void Update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command) {
		memory::Call<void>(this, 3, startframe, validframe, incoming_acknowledged, outgoing_command);
	}
	void GetLocalViewAngles(Vector& view) {
		memory::Call<void>(this, 12, std::ref(view));
	}
	void SetLocalViewAngles(Vector& view) {
		memory::Call<void>(this, 13, std::ref(view));
	}
	void CheckMovingGround(CBasePlayer* player, double frametime) {
		memory::Call<void>(this, 17, player, frametime);
	}
	void SetupMove(CBasePlayer* player, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move) {
		memory::Call<void>(this, 19, player, ucmd, pHelper, move);
	}
	void FinishMove(CBasePlayer* player, CUserCmd* ucmd, CMoveData* move) {
		memory::Call<void>(this, 20, player, ucmd, move);
	}

	std::byte		pad0[0x4];
	std::uintptr_t m_hLastGround;
	bool			m_bInPrediction;
	bool			m_bOldCLPredictValue;
	bool			m_bEnginePaused;

	int				m_nPreviousStartFrame;
	int				m_nIncomingPacketNumber;

	float			m_flLastServerWorldTimeStamp;

	// Last network origin for local player
	struct Split_t
	{
		Split_t()
		{
			m_bFirstTimePredicted = false;
			m_nCommandsPredicted = 0;
			m_nServerCommandsAcknowledged = 0;
			m_bPreviousAckHadErrors = false;
			m_flIdealPitch = 0.0f;

		}

		bool			m_bFirstTimePredicted;
		int				m_nCommandsPredicted;
		int				m_nServerCommandsAcknowledged;
		int				m_bPreviousAckHadErrors;
		float			m_flIdealPitch;
	};

	Split_t				m_Split[1];
};