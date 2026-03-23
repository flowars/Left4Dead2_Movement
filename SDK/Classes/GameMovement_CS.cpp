#include "GameMovement_CS.h"
#include "../Interfaces/CTrace.h"
#include "../Interfaces/GameMovement.h"
#include "../../Core/Interfaces.h"

using namespace interfaces;

//void CS_GameMovement::FinishUnDuck(bool is_server)
//{
//	if (!is_server)
//	{
//		trace_t trace;
//		Vector newOrigin;
//
//		VectorCopy(game_movement->mv->GetAbsOrigin(), newOrigin);
//
//		if (game_movement->player->m_hGroundEntity() != NULL)
//		{
//			newOrigin += VEC_DUCK_HULL_MIN_SCALED(game_movement->player) - VEC_HULL_MIN_SCALED(game_movement->player);
//		}
//		else
//		{
//			// If in air an letting go of croush, make sure we can offset origin to make
//			//  up for uncrouching
//			Vector hullSizeNormal = VEC_HULL_MAX_SCALED(game_movement->player) - VEC_HULL_MIN_SCALED(game_movement->player);
//			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED(game_movement->player) - VEC_DUCK_HULL_MIN_SCALED(game_movement->player);
//
//			Vector viewDelta = -0.5f * (hullSizeNormal - hullSizeCrouch);
//
//			VectorAdd(newOrigin, viewDelta, newOrigin);
//		}
//
//		game_movement->player->m_bDucked() = false;
//		game_movement->player->RemoveFlag(FL_DUCKING);
//		game_movement->player->m_bDucking() = false;
//		game_movement->player->SetViewOffset(game_movement->GetPlayerViewOffset(false));
//		game_movement->player->m_flDucktime() = 0;
//
//		game_movement->mv->SetAbsOrigin(newOrigin);
//
//		// Recategorize position since ducking can change origin
//		game_movement->CategorizePosition();
//	}
//	else
//	{
//		trace_t trace;
//		Vector newOrigin;
//
//		VectorCopy(game_movement_server->mv->GetAbsOrigin(), newOrigin);
//
//		if (game_movement_server->player->m_hGroundEntity() != NULL)
//		{
//			newOrigin += VEC_DUCK_HULL_MIN_SCALED(game_movement_server->player) - VEC_HULL_MIN_SCALED(game_movement_server->player);
//		}
//		else
//		{
//			// If in air an letting go of croush, make sure we can offset origin to make
//			//  up for uncrouching
//			Vector hullSizeNormal = VEC_HULL_MAX_SCALED(game_movement_server->player) - VEC_HULL_MIN_SCALED(game_movement_server->player);
//			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED(game_movement_server->player) - VEC_DUCK_HULL_MIN_SCALED(game_movement_server->player);
//
//			Vector viewDelta = -0.5f * (hullSizeNormal - hullSizeCrouch);
//
//			VectorAdd(newOrigin, viewDelta, newOrigin);
//		}
//
//		game_movement_server->player->m_bDucked() = false;
//		game_movement_server->player->RemoveFlag(FL_DUCKING);
//		game_movement_server->player->m_bDucking() = false;
//		game_movement_server->player->SetViewOffset(game_movement_server->GetPlayerViewOffset(false));
//		game_movement->player->m_flDucktime() = 0;
//
//		game_movement_server->mv->SetAbsOrigin(newOrigin);
//
//		// Recategorize position since ducking can change origin
//		game_movement_server->CategorizePosition();
//	}
//}
//
void CS_GameMovement::FinishDuck(bool is_server)
{
	if (!is_server)
	{
		Vector hullSizeNormal = VEC_HULL_MAX_SCALED(game_movement->player) - VEC_HULL_MIN_SCALED(game_movement->player);
		Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED(game_movement->player) - VEC_DUCK_HULL_MIN_SCALED(game_movement->player);

		Vector viewDelta = 0.5f * (hullSizeNormal - hullSizeCrouch);

		game_movement->player->SetViewOffset(game_movement->GetPlayerViewOffset(true));
		game_movement->player->AddFlag(FL_DUCKING);
		game_movement->player->m_Local().m_bDucking = false;

		if (!game_movement->player->m_Local().m_bDucked)
		{

			Vector org = game_movement->mv->GetAbsOrigin();

			if (game_movement->player->m_hGroundEntity() != NULL)
			{
				org -= VEC_DUCK_HULL_MIN_SCALED(game_movement->player) - VEC_HULL_MIN_SCALED(game_movement->player);
			}
			else
			{
				org += viewDelta;
			}
			game_movement->mv->SetAbsOrigin(org);

			game_movement->player->m_Local().m_bDucked = true;
		}

		// See if we are stuck?
		game_movement->FixPlayerCrouchStuck(true);

		// Recategorize position since ducking can change origin
		game_movement->CategorizePosition();
	}
	else
	{
		Vector hullSizeNormal = VEC_HULL_MAX_SCALED(game_movement_server->player) - VEC_HULL_MIN_SCALED(game_movement_server->player);
		Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED(game_movement_server->player) - VEC_DUCK_HULL_MIN_SCALED(game_movement_server->player);

		Vector viewDelta = 0.5f * (hullSizeNormal - hullSizeCrouch);

		game_movement_server->player->SetViewOffset(game_movement_server->GetPlayerViewOffset(true));
		game_movement_server->player->AddFlag(FL_DUCKING);
		game_movement_server->player->m_Local().m_bDucking = false;

		if (!game_movement_server->player->m_Local().m_bDucked)
		{

			Vector org = game_movement_server->mv->GetAbsOrigin();

			if (game_movement_server->player->m_hGroundEntity() != NULL)
			{
				org -= VEC_DUCK_HULL_MIN_SCALED(game_movement_server->player) - VEC_HULL_MIN_SCALED(game_movement_server->player);
			}
			else
			{
				org += viewDelta;
			}
			game_movement_server->mv->SetAbsOrigin(org);

			game_movement_server->player->m_Local().m_bDucked = true;
		}

		// See if we are stuck?
		game_movement_server->FixPlayerCrouchStuck(true);

		// Recategorize position since ducking can change origin
		game_movement_server->CategorizePosition();
	}
}

//-----------------------------------------------------------------------------
// Simple trace filter
//-----------------------------------------------------------------------------
CTraceFilterSimple::CTraceFilterSimple(const IHandleEntity* passedict, int collisionGroup,
	ShouldHitFunc_t pExtraShouldHitFunc)
{
	m_pPassEnt = passedict;
	m_collisionGroup = collisionGroup;
	m_pExtraShouldHitCheckFunction = pExtraShouldHitFunc;
}

inline void UTIL_TraceHull(const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& hullMin,
	const Vector& hullMax, unsigned int mask, CBaseEntity* ignore,
	int collisionGroup, trace_t* ptr, bool is_server)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd, hullMin, hullMax);
	CTraceFilterWorldOnly traceFilter;

	if (!is_server)
	{
		interfaces::trace->TraceRay(ray, mask, &traceFilter, ptr);
	}
	else
	{
		interfaces::trace_server->TraceRay(ray, mask, &traceFilter, ptr);
	}
}

inline void UTIL_TraceHull(const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& hullMin, const Vector& hullMax, unsigned int mask, ITraceFilter* pFilter, trace_t* ptr, bool is_server)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd, hullMin, hullMax);

	if (!is_server)
	{
		interfaces::trace->TraceRay(ray, mask, pFilter, ptr);
	}
	else
	{
		interfaces::trace_server->TraceRay(ray, mask, pFilter, ptr);
	}
}

bool CS_GameMovement::CanUnduck(bool is_server)
{
	if (!is_server)
	{
		trace_t trace;
		Vector newOrigin;

		VectorCopy(game_movement->mv->GetAbsOrigin(), newOrigin);

		if (game_movement->player->m_hGroundEntity() != NULL)
		{
			newOrigin += VEC_DUCK_HULL_MIN_SCALED(game_movement->player) - VEC_HULL_MIN_SCALED(game_movement->player);
		}
		else
		{
			// If in air an letting go of croush, make sure we can offset origin to make
			//  up for uncrouching
			Vector hullSizeNormal = VEC_HULL_MAX_SCALED(game_movement->player) - VEC_HULL_MIN_SCALED(game_movement->player);
			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED(game_movement->player) - VEC_DUCK_HULL_MIN_SCALED(game_movement->player);

			newOrigin += -0.5f * (hullSizeNormal - hullSizeCrouch);
		}

		UTIL_TraceHull(game_movement->mv->GetAbsOrigin(), newOrigin, VEC_HULL_MIN_SCALED(game_movement->player), VEC_HULL_MAX_SCALED(game_movement->player), MASK_PLAYERSOLID, game_movement->player, COLLISION_GROUP_PLAYER_MOVEMENT, &trace, is_server);

		if (trace.startsolid || (trace.fraction != 1.0f))
			return false;
	}
	else
	{
		trace_t trace;
		Vector newOrigin;

		VectorCopy(game_movement_server->mv->GetAbsOrigin(), newOrigin);

		if (game_movement_server->player->m_hGroundEntity() != NULL)
		{
			newOrigin += VEC_DUCK_HULL_MIN_SCALED(game_movement_server->player) - VEC_HULL_MIN_SCALED(game_movement_server->player);
		}
		else
		{
			// If in air an letting go of croush, make sure we can offset origin to make
			//  up for uncrouching
			Vector hullSizeNormal = VEC_HULL_MAX_SCALED(game_movement_server->player) - VEC_HULL_MIN_SCALED(game_movement_server->player);
			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED(game_movement_server->player) - VEC_DUCK_HULL_MIN_SCALED(game_movement_server->player);

			newOrigin += -0.5f * (hullSizeNormal - hullSizeCrouch);
		}

		UTIL_TraceHull(game_movement_server->mv->GetAbsOrigin(), newOrigin, VEC_HULL_MIN_SCALED(game_movement_server->player), VEC_HULL_MAX_SCALED(game_movement_server->player), MASK_PLAYERSOLID, game_movement_server->player, COLLISION_GROUP_PLAYER_MOVEMENT, &trace, is_server);

		if (trace.startsolid || (trace.fraction != 1.0f))
			return false;
	}
	return true;
}

void CS_GameMovement::HandleDuckingSpeedCrop(bool is_server)
{
	if(!is_server)
	{
		if (game_movement->player->m_iObserverMode() == OBS_MODE_ROAMING)
			return;

		if (!(game_movement->m_iSpeedCropped() & SPEED_CROPPED_DUCK))
		{
			if ((game_movement->mv->m_nButtons & IN_DUCK) || (game_movement->player->m_Local().m_bDucking) || (game_movement->player->m_fFlags() & FL_DUCKING))
			{
				game_movement->mv->m_flForwardMove *= CS_PLAYER_SPEED_DUCK_MODIFIER;
				game_movement->mv->m_flSideMove *= CS_PLAYER_SPEED_DUCK_MODIFIER;
				game_movement->mv->m_flUpMove *= CS_PLAYER_SPEED_DUCK_MODIFIER;
				game_movement->m_iSpeedCropped() |= SPEED_CROPPED_DUCK;
			}
		}
	}
	else
	{
		if (game_movement_server->player->m_iObserverMode() == OBS_MODE_ROAMING)
			return;

		if (!(game_movement_server->m_iSpeedCropped() & SPEED_CROPPED_DUCK))
		{
			if ((game_movement_server->mv->m_nButtons & IN_DUCK) || (game_movement_server->player->m_Local().m_bDucking) || (game_movement_server->player->m_fFlags() & FL_DUCKING))
			{
				game_movement_server->mv->m_flForwardMove *= CS_PLAYER_SPEED_DUCK_MODIFIER;
				game_movement_server->mv->m_flSideMove *= CS_PLAYER_SPEED_DUCK_MODIFIER;
				game_movement_server->mv->m_flUpMove *= CS_PLAYER_SPEED_DUCK_MODIFIER;
				game_movement_server->m_iSpeedCropped() |= SPEED_CROPPED_DUCK;
			}
		}
	}
}

void CS_GameMovement::UpdateDuckJumpEyeOffset(bool is_server)
{
	if (!is_server)
	{
		if (game_movement->player->m_Local().m_flDuckJumpTime != 0.0f)
		{
			float flDuckMilliseconds = MAX(0.0f, GAMEMOVEMENT_DUCK_TIME - (float)game_movement->player->m_Local().m_flDuckJumpTime);
			float flDuckSeconds = flDuckMilliseconds / GAMEMOVEMENT_DUCK_TIME;
			if (flDuckSeconds > TIME_TO_UNDUCK)
			{
				game_movement->player->m_Local().m_flDuckJumpTime = 0.0f;
				game_movement->SetDuckedEyeOffset(0.0f);
			}
			else
			{
				float flDuckFraction = SimpleSpline(1.0f - (flDuckSeconds / TIME_TO_UNDUCK));
				game_movement->SetDuckedEyeOffset(flDuckFraction);
			}
		}
	}
	else
	{
		if (game_movement_server->player->m_Local().m_flDuckJumpTime != 0.0f)
		{
			float flDuckMilliseconds = MAX(0.0f, GAMEMOVEMENT_DUCK_TIME - (float)game_movement_server->player->m_Local().m_flDuckJumpTime);
			float flDuckSeconds = flDuckMilliseconds / GAMEMOVEMENT_DUCK_TIME;
			if (flDuckSeconds > TIME_TO_UNDUCK)
			{
				game_movement_server->player->m_Local().m_flDuckJumpTime = 0.0f;
				game_movement_server->SetDuckedEyeOffset(0.0f);
			}
			else
			{
				float flDuckFraction = SimpleSpline(1.0f - (flDuckSeconds / TIME_TO_UNDUCK));
				game_movement_server->SetDuckedEyeOffset(flDuckFraction);
			}
		}
	}
}

void CS_GameMovement::SetBbox()
{
	interfaces::game_rules->GetViewVectors()->m_vView = Vector(0, 0, 64);
	interfaces::game_rules->GetViewVectors()->m_vHullMin = Vector(-16, -16, 0);		//VEC_HULL_MIN (m_vHullMin)
	interfaces::game_rules->GetViewVectors()->m_vHullMax = Vector(16, 16, 72);		//VEC_HULL_MAX (m_vHullMax)

	interfaces::game_rules->GetViewVectors()->m_vDuckHullMin = Vector(-16, -16, 0);		//VEC_DUCK_HULL_MIN (m_vDuckHullMin)
	interfaces::game_rules->GetViewVectors()->m_vDuckHullMax = Vector(16, 16, 36);		//VEC_DUCK_HULL_MAX	(m_vDuckHullMax)
	interfaces::game_rules->GetViewVectors()->m_vDuckView = Vector(0, 0, 28);			//VEC_DUCK_VIEW		(m_vDuckView)

	interfaces::game_rules->GetViewVectors()->m_vObsHullMin = Vector(-10, -10, -10);		//VEC_OBS_HULL_MIN	(m_vObsHullMin)
	interfaces::game_rules->GetViewVectors()->m_vObsHullMax = Vector(10, 10, 10);		//VEC_OBS_HULL_MAX	(m_vObsHullMax)

	interfaces::game_rules->GetViewVectors()->m_vDeadViewHeight = Vector(0, 0, 14);			//VEC_DEAD_VIEWHEIGHT (m_vDeadViewHeight)
}
