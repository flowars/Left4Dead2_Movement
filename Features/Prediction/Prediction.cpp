#include "Prediction.h"

CBasePlayer** Prediction_Player = nullptr;

void Prediction::Update()
{
	Prediction::BackupPrediction();

	const bool valid = interfaces::client_state->m_nDeltaTick > 0;
	if (valid)
		interfaces::prediction->Update(interfaces::client_state->m_nDeltaTick, valid, interfaces::client_state->last_command_ack, interfaces::client_state->lastoutgoingcommand + interfaces::client_state->chokedcommands);

}

int Prediction::GetTickBase(CUserCmd* cmd)
{
	static int s_nTick = 0;
	static CUserCmd* s_pLastCommand = nullptr;

	if (cmd)
	{
		if (!s_pLastCommand || s_pLastCommand->hasbeenpredicted)
			s_nTick = l4d2::local->m_nTickBase();
		else
			s_nTick++;

		s_pLastCommand = cmd;
	}

	return s_nTick;
}

void UpdateButtonState(CUserCmd* cmd)
{
	const int buttons = cmd->buttons;
	const int local_buttons = *l4d2::local->m_nButtons();
	const int buttons_changed = buttons ^ local_buttons;

	l4d2::local->m_afButtonLast() = local_buttons;
	*l4d2::local->m_nButtons() = buttons;
	l4d2::local->m_afButtonPressed() = buttons_changed & buttons;
	l4d2::local->m_afButtonReleased() = buttons_changed & (~buttons);
}

void Prediction::Begin(CUserCmd* cmd)
{
	if (!interfaces::move_helper)
		return;

	*l4d2::local->m_pCurrentCommand() = cmd;

	if (static bool once = false; !once) {

		interfaces::move_helper->SetHost(l4d2::local);
		//Prediction_Player = *reinterpret_cast<CBasePlayer***>(memory::PatternScan("client.dll", "89 35 ? ? ? ? 0F 57 C0 F3 0F 2A 86 ? ? ? ?") + 2);
		Prediction_RandomSeed = *reinterpret_cast<unsigned int**>(memory::PatternScan("client.dll", "A1 ? ? ? ? 53 56 57 8B 7D 14 8D 4D 14") + 1);

		once = true;
	}

	InPrediction = true;

	*Prediction_RandomSeed = cmd->random_seed;
	//*Prediction_Player = l4d2::local;

	m_flOldCurTime = interfaces::globals->curtime;
	m_flOldFrameTime = interfaces::globals->frametime;
	m_nOldTickCount = interfaces::globals->tickcount;

	const int nTickBase = GetTickBase(cmd);
	const bool Old_FirstTimePredicted = interfaces::prediction->m_Split->m_bFirstTimePredicted;
	const bool Old_InPrediction = interfaces::prediction->m_bInPrediction;

	interfaces::globals->curtime = TICKS_TO_TIME(nTickBase);
	interfaces::globals->frametime = TICK_INTERVAL;
	interfaces::globals->tickcount = nTickBase;

	cmd->buttons |= l4d2::local->m_afButtonForced();
	cmd->buttons &= ~l4d2::local->m_afButtonDisabled();

	interfaces::game_movement->StartTrackPredictionErrors(l4d2::local);

	if (cmd->weaponselect != 0)
	{
		/* guns, money, swag, taliban world */
	}

	if (cmd->impulse)
		l4d2::local->m_nImpulse() = cmd->impulse;

	UpdateButtonState(cmd);

	interfaces::prediction->SetLocalViewAngles(cmd->viewangles);

	interfaces::prediction->SetupMove(l4d2::local, cmd, interfaces::move_helper, &mv);
	interfaces::game_movement->ProcessMovement(l4d2::local, &mv);
	interfaces::prediction->FinishMove(l4d2::local, cmd, &mv);

	interfaces::prediction->m_bInPrediction = Old_InPrediction;
	interfaces::prediction->m_Split->m_bFirstTimePredicted = Old_FirstTimePredicted;
}

void Prediction::Finish()
{
	if (!interfaces::move_helper)
		return;

	interfaces::game_movement->FinishTrackPredictionErrors(l4d2::local);
	interfaces::move_helper->SetHost(nullptr);

	InPrediction = false;

	interfaces::globals->curtime = m_flOldCurTime;
	interfaces::globals->frametime = m_flOldFrameTime;
	interfaces::globals->tickcount = m_nOldTickCount;

	*Prediction_RandomSeed = -1;
	//*Prediction_Player = nullptr;

	interfaces::game_movement->Reset();
}

void Prediction::RestoreEntityToPredictedFrame(int i)
{
	using RestoreEntityToPredictedFramefn = void(__stdcall*)(int, int);
	static auto RestoreEntityToPredictedFrame = reinterpret_cast<RestoreEntityToPredictedFramefn>(memory::PatternScan("client.dll", "55 8B EC 53 8B 5D 08 53 E8 ? ? ? ?"));
	RestoreEntityToPredictedFrame(0, i);
}

