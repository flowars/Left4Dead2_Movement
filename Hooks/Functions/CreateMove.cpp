#include "../Hooks.h"
#include "../Features/Movement/Movement.h"

void __stdcall Hooks::CreateMoveHook(int sequence_number, float input_sample_frametime, bool active)
{
	Hooks::CreateMoveOriginal(interfaces::client, 0, sequence_number, input_sample_frametime, active);

	CUserCmd* cmd = interfaces::input->GetUserCmd(sequence_number);
	CVerifiedUserCmd* verified_cmd = interfaces::input->GetVerifiedCommands(sequence_number);
	if (!cmd || !verified_cmd || !cmd->command_number)
		return;

	l4d2::local = reinterpret_cast<C_BasePlayer*>(interfaces::entity_list->GetClientEntity(interfaces::engine->GetLocalPlayer()));
	l4d2::cmd = cmd;

	if(interfaces::engine->IsInGame() && l4d2::local && l4d2::local->IsAlive())
	{
		Movement::Bhop();
	}

	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->GetChecksum();
}