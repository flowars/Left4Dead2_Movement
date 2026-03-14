#include "CSGO_GameMovement.h"
#include "../../Core/Interfaces.h"
#include "../../Core/Globals.h"

using namespace interfaces;

const Vector& CCSGameMovement::GetPlayerViewOffset(bool ducked)
{
	return ducked ? VEC_DUCK_VIEW : VEC_VIEW;
}

void CCSGameMovement::FinishUnDuck(bool is_server)
{
	if (!is_server)			//For client.dll	
	{

		Vector newOrigin = game_movement->mv->GetAbsOrigin();
		if (game_movement->player->m_hGroundEntity() != NULL || game_movement->player->m_MoveType() == MOVETYPE_LADDER)
		{
			Vector hullMinDelta = VEC_DUCK_HULL_MIN - VEC_HULL_MIN;
			newOrigin += hullMinDelta;
		}
		else
		{
			// If in air an letting go of croush, make sure we can offset origin to make
			//  up for uncrouching
			Vector hullSizeNormal = VEC_HULL_MAX - VEC_HULL_MIN;
			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX - VEC_DUCK_HULL_MIN;
			Vector viewDelta = -0.5f * (hullSizeNormal - hullSizeCrouch);
			newOrigin += viewDelta;
		}

		game_movement->mv->SetAbsOrigin(newOrigin);

		game_movement->player->RemoveFlag(FL_DUCKING | FL_ANIMDUCKING);
		client::m_bDucked = false;
		client::m_bDucking = false;
		client::m_nDuckTimeMsecs = 0; // legacy
		game_movement->player->SetViewOffset(GetPlayerViewOffset(false));

		// Recategorize position since ducking can change origin
		game_movement->CategorizePosition();

		client::m_flDuckAmount = 0.0f;
	}
	else					//For server.dll
	{
		Vector newOrigin = game_movement_server->mv->GetAbsOrigin();
		if (game_movement_server->player->m_hGroundEntity() != NULL || game_movement_server->player->m_MoveType() == MOVETYPE_LADDER)
		{
			Vector hullMinDelta = VEC_DUCK_HULL_MIN - VEC_HULL_MIN;
			newOrigin += hullMinDelta;
		}
		else
		{
			// If in air an letting go of croush, make sure we can offset origin to make
			//  up for uncrouching
			Vector hullSizeNormal = VEC_HULL_MAX - VEC_HULL_MIN;
			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX - VEC_DUCK_HULL_MIN;
			Vector viewDelta = -0.5f * (hullSizeNormal - hullSizeCrouch);
			newOrigin += viewDelta;
		}

		game_movement_server->mv->SetAbsOrigin(newOrigin);

		game_movement_server->player->RemoveFlag(FL_DUCKING | FL_ANIMDUCKING);
		server::m_bDucked = false;
		server::m_bDucking = false;
		server::m_nDuckTimeMsecs = 0; // legacy
		game_movement_server->player->SetViewOffset(GetPlayerViewOffset(false));

		// Recategorize position since ducking can change origin
		game_movement_server->CategorizePosition();

		server::m_flDuckAmount = 0.0f;
	}
}

bool CCSGameMovement::CanUnduck(bool is_server)
{
	if (!is_server)			//For client.dll	
	{
		static bool m_bDuckOverride = false;
		// Can't unduck if we are planting the bomb.
		if (m_bDuckOverride)
			return false;

		// Can always unduck if we are no-clipping
		if (game_movement->player->m_MoveType() == MOVETYPE_NOCLIP)
			return true;

		// Check to see if we would collide on anything if we unducked.
		trace_t trace;
		Vector newOrigin;

		VectorCopy(game_movement->mv->GetAbsOrigin(), newOrigin);

		if (game_movement->player->m_hGroundEntity() != NULL)
		{
			newOrigin += VEC_DUCK_HULL_MIN - VEC_HULL_MIN;
		}
		else
		{
			// If in air an letting go of crouch, make sure we can offset origin to make
			//  up for uncrouching
			Vector hullSizeNormal = VEC_HULL_MAX - VEC_HULL_MIN;
			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX - VEC_DUCK_HULL_MIN;

			newOrigin += -0.5f * (hullSizeNormal - hullSizeCrouch);
		}

		UTIL_TraceHull(game_movement->mv->GetAbsOrigin(), newOrigin, VEC_HULL_MIN, VEC_HULL_MAX, game_movement->PlayerSolidMask(), game_movement->player, COLLISION_GROUP_PLAYER_MOVEMENT, &trace);

		if (trace.startsolid || (trace.fraction != 1.0f))
			return false;
	}
	else					//For server.dll
	{
		static bool m_bDuckOverride = false;
		// Can't unduck if we are planting the bomb.
		if (m_bDuckOverride)
			return false;

		// Can always unduck if we are no-clipping
		if (game_movement_server->player->m_MoveType() == MOVETYPE_NOCLIP)
			return true;

		// Check to see if we would collide on anything if we unducked.
		trace_t trace;
		Vector newOrigin;

		VectorCopy(game_movement_server->mv->GetAbsOrigin(), newOrigin);

		if (game_movement_server->player->m_hGroundEntity() != NULL)
		{
			newOrigin += VEC_DUCK_HULL_MIN - VEC_HULL_MIN;
		}
		else
		{
			// If in air an letting go of crouch, make sure we can offset origin to make
			//  up for uncrouching
			Vector hullSizeNormal = VEC_HULL_MAX - VEC_HULL_MIN;
			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX - VEC_DUCK_HULL_MIN;

			newOrigin += -0.5f * (hullSizeNormal - hullSizeCrouch);
		}

		UTIL_TraceHull(game_movement_server->mv->GetAbsOrigin(), newOrigin, VEC_HULL_MIN, VEC_HULL_MAX, game_movement_server->PlayerSolidMask(), game_movement_server->player, COLLISION_GROUP_PLAYER_MOVEMENT, &trace);

		if (trace.startsolid || (trace.fraction != 1.0f))
			return false;
	}

	return true;
}

void CCSGameMovement::FinishDuck(bool is_server)
{
	if (!is_server)			//For client.dll	
	{
		Vector newOrigin = game_movement->mv->GetAbsOrigin();

		if (game_movement->player->m_hGroundEntity() != NULL || game_movement->player->m_MoveType() == MOVETYPE_LADDER)
		{
			Vector hullMinDelta = VEC_DUCK_HULL_MIN - VEC_HULL_MIN;
			newOrigin -= hullMinDelta;
		}
		else
		{
			Vector hullSizeNormal = VEC_HULL_MAX - VEC_HULL_MIN;
			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX - VEC_DUCK_HULL_MIN;
			Vector viewDelta = -0.5f * (hullSizeNormal - hullSizeCrouch);
			newOrigin -= viewDelta;
		}
		game_movement->mv->SetAbsOrigin(newOrigin);

		game_movement->player->SetViewOffset(GetPlayerViewOffset(true));
		client::m_bDucking = false;
		client::m_bDucked = true;
		client::m_flLastDuckTime = globals->curtime;
		game_movement->player->AddFlag(FL_ANIMDUCKING | FL_DUCKING);

		// See if we are stuck?
		this->FixPlayerCrouchStuck(true);

		// Recategorize position since ducking can change origin
		game_movement->CategorizePosition();

		client::m_flDuckAmount = 1.0f;
	}
	else					//For server.dll
	{
		Vector newOrigin = game_movement_server->mv->GetAbsOrigin();

		if (game_movement_server->player->m_hGroundEntity() != NULL || game_movement_server->player->m_MoveType() == MOVETYPE_LADDER)
		{
			Vector hullMinDelta = VEC_DUCK_HULL_MIN - VEC_HULL_MIN;
			newOrigin -= hullMinDelta;
		}
		else
		{
			Vector hullSizeNormal = VEC_HULL_MAX - VEC_HULL_MIN;
			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX - VEC_DUCK_HULL_MIN;
			Vector viewDelta = -0.5f * (hullSizeNormal - hullSizeCrouch);
			newOrigin -= viewDelta;
		}
		game_movement_server->mv->SetAbsOrigin(newOrigin);

		game_movement_server->player->SetViewOffset(GetPlayerViewOffset(true));
		server::m_bDucking = false;
		server::m_bDucked = true;
		server::m_flLastDuckTime = globals->curtime;
		game_movement_server->player->AddFlag(FL_ANIMDUCKING | FL_DUCKING);

		// See if we are stuck?
		this->FixPlayerCrouchStuck(true);

		// Recategorize position since ducking can change origin
		game_movement_server->CategorizePosition();

		server::m_flDuckAmount = 1.0f;
	}
}

void CCSGameMovement::HandleDuckingSpeedCrop(float duckFraction, bool is_server)
{
	if (!is_server)			//For client.dll	
	{
		static int				m_iSpeedCropped;

		if (game_movement->player->m_iObserverMode() == OBS_MODE_ROAMING)
			return;

		if (!(m_iSpeedCropped & SPEED_CROPPED_DUCK))
		{
			if ((game_movement->mv->m_nButtons & IN_DUCK) || (client::m_bDucking) || (game_movement->player->m_fFlags() & FL_DUCKING))
			{
				float duckSpeedModifier = GetDuckSpeedModifier(duckFraction);
				//DevMsg( "duckSpeedModifier = %f\n", duckSpeedModifier );
				game_movement->mv->m_flForwardMove *= duckSpeedModifier;
				game_movement->mv->m_flSideMove *= duckSpeedModifier;
				game_movement->mv->m_flUpMove *= duckSpeedModifier;
				game_movement->mv->m_flMaxSpeed *= duckSpeedModifier;
				m_iSpeedCropped |= SPEED_CROPPED_DUCK;
			}
		}
	}
	else					//For server.dll
	{
		static int				m_iSpeedCropped_s;

		if (game_movement_server->player->m_iObserverMode() == OBS_MODE_ROAMING)
			return;

		if (!(m_iSpeedCropped_s & SPEED_CROPPED_DUCK))
		{
			if ((game_movement_server->mv->m_nButtons & IN_DUCK) || (server::m_bDucking) || (game_movement_server->player->m_fFlags() & FL_DUCKING))
			{
				float duckSpeedModifier = GetDuckSpeedModifier(duckFraction);
				//DevMsg( "duckSpeedModifier = %f\n", duckSpeedModifier );
				game_movement_server->mv->m_flForwardMove *= duckSpeedModifier;
				game_movement_server->mv->m_flSideMove *= duckSpeedModifier;
				game_movement_server->mv->m_flUpMove *= duckSpeedModifier;
				game_movement_server->mv->m_flMaxSpeed *= duckSpeedModifier;
				m_iSpeedCropped_s |= SPEED_CROPPED_DUCK;
			}
		}
	}
}

void CCSGameMovement::FixPlayerCrouchStuck(bool upward, bool is_server)
{
	if (!is_server)					//client
	{
		EntityHandle_t hitent;
		int i;
		Vector test;
		trace_t dummy;

		int direction = upward ? 1 : 0;

		hitent = TestPlayerPosition(game_movement->mv->GetAbsOrigin(), COLLISION_GROUP_PLAYER_MOVEMENT, dummy, is_server);
		if (hitent == INVALID_ENTITY_HANDLE)
			return;

		VectorCopy(game_movement->mv->GetAbsOrigin(), test);
		for (i = 0; i < 36; i++)
		{
			Vector org = game_movement->mv->GetAbsOrigin();
			org.z += direction;
			game_movement->mv->SetAbsOrigin(org);
			hitent = TestPlayerPosition(game_movement->mv->GetAbsOrigin(), COLLISION_GROUP_PLAYER_MOVEMENT, dummy, is_server);
			if (hitent == INVALID_ENTITY_HANDLE)
				return;
		}

		game_movement->mv->SetAbsOrigin(test); // Failed
	}
	else							//server
	{
		EntityHandle_t hitent;
		int i;
		Vector test;
		trace_t dummy;

		int direction = upward ? 1 : 0;

		hitent = TestPlayerPosition(game_movement_server->mv->GetAbsOrigin(), COLLISION_GROUP_PLAYER_MOVEMENT, dummy, is_server);
		if (hitent == INVALID_ENTITY_HANDLE)
			return;

		VectorCopy(game_movement_server->mv->GetAbsOrigin(), test);
		for (i = 0; i < 36; i++)
		{
			Vector org = game_movement_server->mv->GetAbsOrigin();
			org.z += direction;
			game_movement_server->mv->SetAbsOrigin(org);
			hitent = TestPlayerPosition(game_movement_server->mv->GetAbsOrigin(), COLLISION_GROUP_PLAYER_MOVEMENT, dummy, is_server);
			if (hitent == INVALID_ENTITY_HANDLE)
				return;
		}

		game_movement_server->mv->SetAbsOrigin(test); // Failed
	}
}

CBaseHandle CCSGameMovement::TestPlayerPosition(const Vector& pos, int collisionGroup, trace_t& pm, bool is_server)
{
	if(!is_server)
	{
		Ray_t ray;
		ray.Init(pos, pos, VEC_HULL_MIN, VEC_HULL_MAX);
		UTIL_TraceRay(ray, MASK_PLAYERSOLID, l4d2::local , collisionGroup, &pm);
		if ((pm.contents & MASK_PLAYERSOLID) && pm.m_pEnt)
		{
			return pm.m_pEnt->GetRefEHandle();
		}
		else
		{
			return INVALID_EHANDLE;
		}
	}
	else
	{
		Ray_t ray;
		ray.Init(pos, pos, VEC_HULL_MIN, VEC_HULL_MAX);
		UTIL_TraceRay(ray, MASK_PLAYERSOLID, l4d2::local, collisionGroup, &pm);
		if ((pm.contents & MASK_PLAYERSOLID) && pm.m_pEnt)
		{
			return pm.m_pEnt->GetRefEHandle();
		}
		else
		{
			return INVALID_EHANDLE;
		}
	}
}

inline void UTIL_TraceRay(const Ray_t& ray, unsigned int mask, CBaseEntity* ignore, int collisionGroup, trace_t* ptr)
{
	ITraceFilter traceFilter(ignore);

	trace->TraceRay(ray, mask, &traceFilter, ptr);
}

inline void UTIL_TraceHull(const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& hullMin, const Vector& hullMax, unsigned int mask, CBaseEntity* ignore, int collisionGroup, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd, hullMin, hullMax);
	ITraceFilter traceFilter(ignore);

	trace->TraceRay(ray, mask, &traceFilter, ptr);
}
