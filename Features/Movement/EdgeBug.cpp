#include "Movement.h"

std::vector<Vector> ebpos;
bool check_edge_bug(CUserCmd* cmd, bool& brk) {
	Vector unpredicted_velocity = Prediction_backup::vec_velocity;
	Vector predicted_velocity = l4d2::local->m_vecVelocity();
	int predicted_flags = l4d2::local->m_fFlags();

	static auto Sv_gravity = interfaces::cvar->FindVar("sv_gravity");
	auto sv_gravity = Sv_gravity->GetFloat();

	float z_vel_pred = round(predicted_velocity.z);
	if (z_vel_pred >= 0.f || (predicted_flags & FL_ONGROUND)) {
		brk = true;
		return false;
	}
	else if (unpredicted_velocity.z < 0.f && predicted_velocity.z > unpredicted_velocity.z && predicted_velocity.z < 0.f) {
		int z_vel = predicted_velocity.z;

		Prediction::Begin(cmd);
		Prediction::Finish();

		float rounded_vel = round(-sv_gravity * interfaces::globals->interval_per_tick) + z_vel;
		float unpredicted_vel = round(l4d2::local->m_vecVelocity().z);

		if (rounded_vel == unpredicted_vel || (unpredicted_vel == 0.f && (l4d2::local->m_fFlags() & FL_ONGROUND))) {
			return true;
		}
		else {
			brk = true;
			return false;
		}
	}
	return false;
}
/* Player when he jumps, always stay in ducked bbox, so it should be recoded only for stand || crouch state, but rn im too lazy */
void Movement::EdgeBug()
{
	if (Config::Movements::EdgeBugNoDamage && Detect_EB)
	{
		/* set falldamage to 0 */

		CBasePlayer* server_player = nullptr;
		server_player = server_player->UTIL_PlayerByIndex(1);

		if (server_player)
			server_player->m_flFallVelocity_server() = 0.f;
	}

	if (!(Config::Movements::bEdgeBug) || !CheckKey(Config::Movements::kEdgeBug)) {
		EdgeBug_data.DetectTick = 0;
		EdgeBug_data.EdgebugTick = 0;
		EdgeBug_data.Ticks_Left = 0;
		return;
	}

	if ((Prediction_backup::flags & FL_ONGROUND) || l4d2::local->m_MoveType() == MOVETYPE_NOCLIP || l4d2::local->m_MoveType() == MOVETYPE_OBSERVER || l4d2::local->m_MoveType() == MOVETYPE_LADDER) {
		EdgeBug_data.DetectTick = 0;
		EdgeBug_data.EdgebugTick = 0;
		EdgeBug_data.Ticks_Left = 0;
		return;
	}

	if (!EdgeBug_data.Ticks_Left) {
		static ConVar* get_yaw = interfaces::cvar->FindVar("m_yaw");
		static ConVar* get_sens = interfaces::cvar->FindVar("sensitivity");

		interfaces::cvar->FindVar("sensitivity")->m_fnChangeCallbacks.RemoveAll();
		interfaces::cvar->FindVar("m_yaw")->m_fnChangeCallbacks.RemoveAll();

		const float m_yaw = get_yaw->GetFloat();
		const float sensitivity = get_sens->GetFloat();
		int ticklimit = Config::Movements::EdgeBugTicks;
		float yawdelta = std::clamp(l4d2::cmd->mousedx * m_yaw * sensitivity, -30.f, 30.f);
		int pred_rounds = (Config::Movements::ExtendedEdgeBugPaths && yawdelta != 0.f) ? 4 : 2;
		float originalfmove = l4d2::cmd->forwardmove;
		float originalsmove = l4d2::cmd->sidemove;
		Vector originalangles = l4d2::cmd->viewangles;
		EdgeBug_data.StartingYaw = originalangles.y;


		ebpos.clear();
		ebpos.push_back(l4d2::local->m_vecOrigin());

		for (int round = 0; round < pred_rounds; ++round) {
			if (round > 1) {
				float max_delta = yawdelta;
				for (yawdelta = max_delta / Config::Movements::EdgeBugPaths; abs(yawdelta) <= abs(max_delta); yawdelta += max_delta / Config::Movements::EdgeBugPaths) {
					CUserCmd predictcmd = *l4d2::cmd;
					if (round == 2) {
						EdgeBug_data.Crouched = true;
						predictcmd.buttons |= IN_DUCK;
						EdgeBug_data.Strafing = true;
						predictcmd.viewangles.y = originalangles.y;
						predictcmd.forwardmove = originalfmove;
						predictcmd.sidemove = originalsmove;
					}
					else if (round == 3) {
						EdgeBug_data.Crouched = false;
						predictcmd.buttons &= ~IN_DUCK;
						EdgeBug_data.Strafing = true;
						predictcmd.viewangles.y = originalangles.y;
						predictcmd.forwardmove = originalfmove;
						predictcmd.sidemove = originalsmove;
					}

					Prediction::RestoreEntityToPredictedFrame(interfaces::prediction->m_Split->m_nCommandsPredicted - 1);

					for (int t = 1; t <= ticklimit; ++t) {
						if (round > 1)
							predictcmd.viewangles.y = Math::NormalizeYaw(originalangles.y + (yawdelta * t));

						if (abs(predictcmd.viewangles.y - EdgeBug_data.StartingYaw) > Config::Movements::EdgeBugAngle)
							break;

						Prediction::Begin(&predictcmd);
						Prediction::Finish();

						if (l4d2::local->m_vecVelocity().z > 0.f || l4d2::local->m_vecVelocity().Length2D() == 0.f || l4d2::local->m_MoveType() == MOVETYPE_LADDER)
							break;

						ebpos.push_back(l4d2::local->m_vecOrigin());

						bool br = false;

						if (check_edge_bug(&predictcmd, br)) {
							EdgeBug_data.Ticks_Left = t;
							EdgeBug_data.EbLength = t;
							EdgeBug_data.EdgebugTick = interfaces::globals->tickcount + t;
							EdgeBug_data.DetectTick = interfaces::globals->tickcount;
							EdgeBug_data.Forwardmove = predictcmd.forwardmove;
							EdgeBug_data.Sidemove = predictcmd.sidemove;
							EdgeBug_data.YawDelta = yawdelta;
							break;
						}

						if (br)
							break;
					}
				}
			}
			else {
				CUserCmd predictcmd = *l4d2::cmd;
				if (round == 0) {
					EdgeBug_data.Crouched = true;
					predictcmd.buttons |= IN_DUCK;
					EdgeBug_data.Strafing = false;
					predictcmd.forwardmove = 0.f;
					predictcmd.sidemove = 0.f;
				}
				else if (round == 1) {
					EdgeBug_data.Crouched = false;
					predictcmd.buttons &= ~IN_DUCK;
					EdgeBug_data.Strafing = false;
					predictcmd.forwardmove = 0.f;
					predictcmd.sidemove = 0.f;
				}

				Prediction::RestoreEntityToPredictedFrame(interfaces::prediction->m_Split->m_nCommandsPredicted - 1);

				for (int t = 1; t <= ticklimit; ++t) {

					Prediction::Begin(&predictcmd);
					Prediction::Finish();

					if (l4d2::local->m_vecVelocity().z > 0.f || l4d2::local->m_vecVelocity().Length2D() == 0.f || l4d2::local->m_MoveType() == MOVETYPE_LADDER)
						break;

					ebpos.push_back(l4d2::local->m_vecOrigin());

					bool br = false;

					if (check_edge_bug(&predictcmd, br)) {
						EdgeBug_data.Ticks_Left = t;
						EdgeBug_data.EbLength = t;
						EdgeBug_data.EdgebugTick = interfaces::globals->tickcount + t;
						EdgeBug_data.DetectTick = interfaces::globals->tickcount;
						EdgeBug_data.Forwardmove = predictcmd.forwardmove;
						EdgeBug_data.Sidemove = predictcmd.sidemove;
						EdgeBug_data.YawDelta = yawdelta;
						break;
					}

					if (br)
						break;
				}
			}

			if (EdgeBug_data.Ticks_Left)
				break;
		}
	}

	if (EdgeBug_data.Ticks_Left) {
		if (EdgeBug_data.Ticks_Left == 1) {
			//insane chatprintf
			Detect_EB = true;
		}

		Should_EB = true;


		if (EdgeBug_data.Crouched)
			l4d2::cmd->buttons |= IN_DUCK;
		else
			l4d2::cmd->buttons &= ~IN_DUCK;


		if (EdgeBug_data.Strafing) {
			l4d2::cmd->forwardmove = EdgeBug_data.Forwardmove;
			l4d2::cmd->sidemove = EdgeBug_data.Sidemove;
			l4d2::cmd->viewangles.y = Math::NormalizeYaw(EdgeBug_data.StartingYaw + (EdgeBug_data.YawDelta * (EdgeBug_data.EbLength - (EdgeBug_data.Ticks_Left - 1))));
		}
		else {
			l4d2::cmd->forwardmove = 0.f;
			l4d2::cmd->sidemove = 0.f;
		}
		EdgeBug_data.Ticks_Left--;
	}
	else {
		Should_EB = false;
		Detect_EB = false;
	}
}

void Movement::FixMovement(Vector& angle)
{
	if (!l4d2::local)
		return;

	Vector move, dir;
	Vector move_angle;
	float delta, len;

	if (!(l4d2::local->m_fFlags() & FL_ONGROUND) && l4d2::cmd->viewangles.z != 0 && l4d2::cmd->buttons & IN_ATTACK)
		l4d2::cmd->sidemove = 0;

	move = { l4d2::cmd->forwardmove, l4d2::cmd->sidemove, 0 };

	len = move.NormalizeMovement();

	if (!len)
		return;

	Math::VectorAngles(move, move_angle);

	delta = (l4d2::cmd->viewangles.y - angle.y);

	move_angle.y += delta;

	Math::AngleVectors(move_angle, &dir);

	dir *= len;

	if (l4d2::local->m_MoveType() == MOVETYPE_LADDER) {
		if (l4d2::cmd->viewangles.x >= 45 && angle.x < 45 && std::abs(delta) <= 65)
			dir.x = -dir.x;

		l4d2::cmd->forwardmove = dir.x;
		l4d2::cmd->sidemove = dir.y;

		if (l4d2::cmd->forwardmove > 200)
			l4d2::cmd->buttons |= IN_FORWARD;

		else if (l4d2::cmd->forwardmove < -200)
			l4d2::cmd->buttons |= IN_BACK;

		if (l4d2::cmd->sidemove > 200)
			l4d2::cmd->buttons |= IN_MOVERIGHT;

		else if (l4d2::cmd->sidemove < -200)
			l4d2::cmd->buttons |= IN_MOVELEFT;
	}
	else {
		if (l4d2::cmd->viewangles.x < -90 || l4d2::cmd->viewangles.x > 90)
			dir.x = -dir.x;

		l4d2::cmd->forwardmove = dir.x;
		l4d2::cmd->sidemove = dir.y;
	}

	l4d2::cmd->forwardmove = std::clamp(l4d2::cmd->forwardmove, -450.f, 450.f);
	l4d2::cmd->sidemove = std::clamp(l4d2::cmd->sidemove, -450.f, 450.f);
	l4d2::cmd->upmove = std::clamp(l4d2::cmd->upmove, -320.f, 320.f);

	if (l4d2::local->m_MoveType() != MOVETYPE_LADDER)
		l4d2::cmd->buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT);
}
