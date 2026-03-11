#include "../Hooks.h"
#include "../Features/Movement/Movement.h"
#include "../../Features/Prediction/Prediction.h"

void __stdcall Hooks::CreateMoveHook(int sequence_number, float input_sample_frametime, bool active)
{
	Hooks::CreateMoveOriginal(interfaces::client, 0, sequence_number, input_sample_frametime, active);

	CUserCmd* cmd = interfaces::input->GetUserCmd(sequence_number);
	CVerifiedUserCmd* verified_cmd = interfaces::input->GetVerifiedCommands(sequence_number);
	if (!cmd || !verified_cmd || !cmd->command_number)
		return;

	l4d2::local = reinterpret_cast<CBasePlayer*>(interfaces::entity_list->GetClientEntity(interfaces::engine->GetLocalPlayer()));
	l4d2::cmd = cmd;

	Vector SavedViewAngles = cmd->viewangles;

	if(interfaces::engine->IsInGame() && l4d2::local && l4d2::local->IsAlive())
	{
		Prediction::Update();

		Movement::Bhop();

		printf("m_flFallVelocity: %.2f \n", l4d2::local->m_flFallVelocity());

		Prediction::Begin(cmd);
		{
			/* Insane rage functions, yassir assistant traceray edge bugs/align and etc */
		}
		Prediction::Finish();

		Movement::EdgeJump();

		/* useless functions, but whatever */
		{
			Movement::LongJump();
			Movement::MiniJump();
		}

		Movement::FixMovement(SavedViewAngles);
		Movement::EdgeBug();

	}

	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->GetChecksum();
}