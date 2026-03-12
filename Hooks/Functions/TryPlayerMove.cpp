#include "../Hooks.h"
#include "../../SDK/Interfaces/GameMovement.h"

#define MINIMUM_MOVE_FRACTION			0.0001f
#define EFFECTIVELY_HORIZONTAL_NORMAL_Z	0.0001f
#define	STOP_EPSILON		0.1
#define	MAX_CLIP_PLANES		5
#define PLAYER_MAX_SAFE_FALL_SPEED	580 // approx 20 feet
Vector vec3_origin = { 0.f, 0.f, 0.f };

//Client
int __fastcall Hooks::TryPlayerMoveClient(void* ecx, void* ebp, Vector* pFirstDest, trace_t* pFirstTrace)
{
	if (!Config::Movements::bTryPlayerMove || !interfaces::engine->IsDedicatedServer())
		return Hooks::TryPlayerMoveClientOriginal(ecx, ebp, pFirstDest, pFirstTrace);

	int			bumpcount, numbumps;
	Vector		dir;
	float		d;
	int			numplanes;
	Vector		planes[MAX_CLIP_PLANES];
	Vector		primal_velocity, original_velocity;
	Vector      new_velocity;
	int			i, j;
	trace_t	pm;
	Vector		end;
	float		time_left, allFraction;
	int			blocked;

	numbumps = 4;           // Bump up to four times

	blocked = 0;           // Assume not blocked
	numplanes = 0;           //  and not sliding along any planes

	VectorCopy(interfaces::game_movement->mv->m_vecVelocity, original_velocity);  // Store original velocity
	VectorCopy(interfaces::game_movement->mv->m_vecVelocity, primal_velocity);

	allFraction = 0;
	time_left = interfaces::globals->interval_per_tick;   // Total time for this movement operation.

	new_velocity.Init();
	for (bumpcount = 0; bumpcount < numbumps; bumpcount++)
	{
		if (interfaces::game_movement->mv->m_vecVelocity.Length() == 0.0)
			break;

		// Assume we can move all the way from the current origin to the
		//  end point.
		VectorMA(interfaces::game_movement->mv->GetAbsOrigin(), time_left, interfaces::game_movement->mv->m_vecVelocity, end);

		// See if we can make it from origin to end point.
		if (false)
		{
			// If their velocity Z is 0, then we can avoid an extra trace here during WalkMove.
			if (pFirstDest && (end == *pFirstDest))
			{
				pm = *pFirstTrace;
			}
			else
			{
#if defined( PLAYER_GETTING_STUCK_TESTING )
				trace_t foo;
				TracePlayerBboxCSGO(interfaces::game_movement->mv->GetAbsOrigin(), interfaces::game_movement->mv->GetAbsOrigin(), MASK_PLAYERSOLID, COLLISION_GROUP_PLAYER_MOVEMENT, foo);
				if (foo.startsolid || foo.fraction != 1.0f)
				{
					Msg("bah\n");
				}
#endif
				interfaces::game_movement->TracePlayerBBox(interfaces::game_movement->mv->GetAbsOrigin(), end, interfaces::game_movement->PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, pm);
			}
		}
		else
		{
			interfaces::game_movement->TracePlayerBBox(interfaces::game_movement->mv->GetAbsOrigin(), end, interfaces::game_movement->PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, pm);
		}

		if (pm.fraction > 0 && pm.fraction < MINIMUM_MOVE_FRACTION)
		{
			//HACK: extremely tiny move fractions cause problems in later computations that determine values using portions of distance moved.
			pm.fraction = 0;
		}

		allFraction += pm.fraction;

		// If we started in a solid object, or we were in solid space
		//  the whole way, zero out our velocity and return that we
		//  are blocked by floor and wall.
		if (pm.allsolid)
		{
			// entity is trapped in another solid
			VectorCopy(vec3_origin, interfaces::game_movement->mv->m_vecVelocity);
			return 4;
		}

		// If we moved some portion of the total distance, then
		//  copy the end position into the pmove.origin and 
		//  zero the plane counter.
		if (pm.fraction > 0)
		{
			if (numbumps > 0 && pm.fraction == 1)
			{
				// There's a precision issue with terrain tracing that can cause a swept box to successfully trace
				// when the end position is stuck in the triangle.  Re-run the test with an uswept box to catch that
				// case until the bug is fixed.
				// If we detect getting stuck, don't allow the movement
				trace_t stuck;
				//TracePlayerBboxCSGO(pm.endpos, pm.endpos, MASK_PLAYERSOLID, COLLISION_GROUP_PLAYER_MOVEMENT, stuck);
				interfaces::game_movement->TracePlayerBBox(pm.endpos, pm.endpos, interfaces::game_movement->PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, stuck);
				if (stuck.startsolid || stuck.fraction != 1.0f)
				{
					//Msg( "Player will become stuck!!!\n" );
					VectorCopy(vec3_origin, interfaces::game_movement->mv->m_vecVelocity);
					break;
				}
			}

#if defined( PLAYER_GETTING_STUCK_TESTING )
			trace_t foo;
			TracePlayerBboxCSGO(pm.endpos, pm.endpos, MASK_PLAYERSOLID, COLLISION_GROUP_PLAYER_MOVEMENT, foo);
			if (foo.startsolid || foo.fraction != 1.0f)
			{
				Msg("Player will become stuck!!!\n");
			}
#endif
			// actually covered some distance
			interfaces::game_movement->mv->SetAbsOrigin(pm.endpos);
			VectorCopy(interfaces::game_movement->mv->m_vecVelocity, original_velocity);
			numplanes = 0;
		}

		// If we covered the entire distance, we are done
		//  and can return.
		if (pm.fraction == 1)
		{
			break;		// moved the entire distance
		}

		// Save entity that blocked us (since fraction was < 1.0)
		//  for contact
		// Add it if it's not already in the list!!!
		interfaces::move_helper->AddToTouched(pm, interfaces::game_movement->mv->m_vecVelocity);

		// If the plane we hit has a high z component in the normal, then
		//  it's probably a floor
		if (pm.plane.normal[2] > 0.7)
		{
			blocked |= 1;		// floor
		}
		// If the plane has a zero z component in the normal, then it's a 
		//  step or wall
		if (abs(pm.plane.normal[2]) < EFFECTIVELY_HORIZONTAL_NORMAL_Z)
		{
			pm.plane.normal[2] = 0;
			blocked |= 2;		// step / wall
		}

		// Reduce amount of m_flFrameTime left by total time left * fraction
		//  that we covered.
		time_left -= time_left * pm.fraction;

		// Did we run out of planes to clip against?
		if (numplanes >= MAX_CLIP_PLANES)
		{
			// this shouldn't really happen
			//  Stop our movement if so.
			VectorCopy(vec3_origin, interfaces::game_movement->mv->m_vecVelocity);
			//Con_DPrintf("Too many planes 4\n");

			break;
		}

		// Set up next clipping plane
		VectorCopy(pm.plane.normal, planes[numplanes]);
		numplanes++;

		// modify original_velocity so it parallels all of the clip planes
		//

		// reflect player velocity 
		// Only give this a try for first impact plane because you can get yourself stuck in an acute corner by jumping in place
		//  and pressing forward and nobody was really using this bounce/reflection feature anyway...
		if (numplanes == 1 &&
			interfaces::game_movement->player->m_MoveType() == MOVETYPE_WALK &&
			interfaces::game_movement->player->m_hGroundEntity() == NULL)
		{
			for (i = 0; i < numplanes; i++)
			{
				if (planes[i][2] > 0.7)
				{
					// floor or slope
					ClipVelocity(original_velocity, planes[i], new_velocity, 1);
					VectorCopy(new_velocity, original_velocity);
				}
				else
				{
					ClipVelocity(original_velocity, planes[i], new_velocity, 1.0 + interfaces::cvar->FindVar("sv_bounce")->GetFloat() * (1 - interfaces::game_movement->player->m_surfaceFriction()));
				}
			}

			VectorCopy(new_velocity, interfaces::game_movement->mv->m_vecVelocity);
			VectorCopy(new_velocity, original_velocity);
		}
		else
		{
			for (i = 0; i < numplanes; i++)
			{
				ClipVelocity(original_velocity, planes[i], interfaces::game_movement->mv->m_vecVelocity, 1);

				for (j = 0; j < numplanes; j++)
				{
					if (j != i)
					{
						// Are we now moving against this plane?
						if (interfaces::game_movement->mv->m_vecVelocity.Dot(planes[j]) < 0)
							break;	// not ok
					}
				}

				if (j == numplanes)  // Didn't have to clip, so we're ok
					break;
			}

			// Did we go all the way through plane set
			if (i != numplanes)
			{	// go along this plane
				// pmove.velocity is set in clipping call, no need to set again.
				;
			}
			else
			{	// go along the crease
				if (numplanes != 2)
				{
					VectorCopy(vec3_origin, interfaces::game_movement->mv->m_vecVelocity);
					break;
				}
				CrossProduct(planes[0], planes[1], dir);
				dir.NormalizeInPlace();
				d = dir.Dot(interfaces::game_movement->mv->m_vecVelocity);
				VectorScale(dir, d, interfaces::game_movement->mv->m_vecVelocity);
			}

			//
			// if original velocity is against the original velocity, stop dead
			// to avoid tiny occilations in sloping corners
			//
			d = interfaces::game_movement->mv->m_vecVelocity.Dot(primal_velocity);
			if (d <= 0)
			{
				//Con_DPrintf("Back\n");
				VectorCopy(vec3_origin, interfaces::game_movement->mv->m_vecVelocity);
				break;
			}
		}
	}

	if (allFraction == 0)
	{
		VectorCopy(vec3_origin, interfaces::game_movement->mv->m_vecVelocity);
	}

	// Check if they slammed into a wall
	float fSlamVol = 0.0f;

	float fLateralStoppingAmount = primal_velocity.Length2D() - interfaces::game_movement->mv->m_vecVelocity.Length2D();
	if (fLateralStoppingAmount > PLAYER_MAX_SAFE_FALL_SPEED * 2.0f)
	{
		fSlamVol = 1.0f;
	}
	else if (fLateralStoppingAmount > PLAYER_MAX_SAFE_FALL_SPEED)
	{
		fSlamVol = 0.85f;
	}

	if (fSlamVol > 0.0f)
	{
		interfaces::game_movement->PlayerRoughLandingEffects(fSlamVol);
	}

	return blocked;

	//return Hooks::TryPlayerMoveClientOriginal(ecx, ebp, pFirstDest, pFirstTrace);
}

//Server
int __fastcall Hooks::TryPlayerMoveServer(void* ecx, void* ebp, Vector* pFirstDest, trace_t* pFirstTrace)
{
	if (!Config::Movements::bTryPlayerMove || !interfaces::engine->IsDedicatedServer())
		return Hooks::TryPlayerMoveServerOriginal(ecx, ebp, pFirstDest, pFirstTrace);

	int			bumpcount, numbumps;
	Vector		dir;
	float		d;
	int			numplanes;
	Vector		planes[MAX_CLIP_PLANES];
	Vector		primal_velocity, original_velocity;
	Vector      new_velocity;
	int			i, j;
	trace_t	pm;
	Vector		end;
	float		time_left, allFraction;
	int			blocked;

	numbumps = 4;           // Bump up to four times

	blocked = 0;           // Assume not blocked
	numplanes = 0;           //  and not sliding along any planes

	VectorCopy(interfaces::game_movement_server->mv->m_vecVelocity, original_velocity);  // Store original velocity
	VectorCopy(interfaces::game_movement_server->mv->m_vecVelocity, primal_velocity);

	allFraction = 0;
	time_left = interfaces::globals->interval_per_tick;   // Total time for this movement operation.

	new_velocity.Init();
	for (bumpcount = 0; bumpcount < numbumps; bumpcount++)
	{
		if (interfaces::game_movement_server->mv->m_vecVelocity.Length() == 0.0)
			break;

		// Assume we can move all the way from the current origin to the
		//  end point.
		VectorMA(interfaces::game_movement_server->mv->GetAbsOrigin(), time_left, interfaces::game_movement_server->mv->m_vecVelocity, end);

		// See if we can make it from origin to end point.
		if (false)
		{
			// If their velocity Z is 0, then we can avoid an extra trace here during WalkMove.
			if (pFirstDest && (end == *pFirstDest))
			{
				pm = *pFirstTrace;
			}
			else
			{
#if defined( PLAYER_GETTING_STUCK_TESTING )
				trace_t foo;
				TracePlayerBboxCSGO_server(interfaces::game_movement_server->mv->GetAbsOrigin(), interfaces::game_movement_server->mv->GetAbsOrigin(), MASK_PLAYERSOLID, COLLISION_GROUP_PLAYER_MOVEMENT, foo);
				if (foo.startsolid || foo.fraction != 1.0f)
				{
					Msg("bah\n");
				}
#endif
				interfaces::game_movement_server->TracePlayerBBox(interfaces::game_movement_server->mv->GetAbsOrigin(), end, interfaces::game_movement_server->PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, pm);
			}
		}
		else
		{
			interfaces::game_movement_server->TracePlayerBBox(interfaces::game_movement_server->mv->GetAbsOrigin(), end, interfaces::game_movement_server->PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, pm);
		}

		if (pm.fraction > 0 && pm.fraction < MINIMUM_MOVE_FRACTION)
		{
			//HACK: extremely tiny move fractions cause problems in later computations that determine values using portions of distance moved.
			pm.fraction = 0;
		}

		allFraction += pm.fraction;

		// If we started in a solid object, or we were in solid space
		//  the whole way, zero out our velocity and return that we
		//  are blocked by floor and wall.
		if (pm.allsolid)
		{
			// entity is trapped in another solid
			VectorCopy(vec3_origin, interfaces::game_movement_server->mv->m_vecVelocity);
			return 4;
		}

		// If we moved some portion of the total distance, then
		//  copy the end position into the pmove.origin and 
		//  zero the plane counter.
		if (pm.fraction > 0)
		{
			if (numbumps > 0 && pm.fraction == 1)
			{
				// There's a precision issue with terrain tracing that can cause a swept box to successfully trace
				// when the end position is stuck in the triangle.  Re-run the test with an uswept box to catch that
				// case until the bug is fixed.
				// If we detect getting stuck, don't allow the movement
				trace_t stuck;
				interfaces::game_movement_server->TracePlayerBBox(pm.endpos, pm.endpos, interfaces::game_movement_server->PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, stuck);
				if (stuck.startsolid || stuck.fraction != 1.0f)
				{
					//Msg( "Player will become stuck!!!\n" );
					VectorCopy(vec3_origin, interfaces::game_movement_server->mv->m_vecVelocity);
					break;
				}
			}

#if defined( PLAYER_GETTING_STUCK_TESTING )
			trace_t foo;
			TracePlayerBboxCSGO_server(pm.endpos, pm.endpos, MASK_PLAYERSOLID, COLLISION_GROUP_PLAYER_MOVEMENT, foo);
			if (foo.startsolid || foo.fraction != 1.0f)
			{
				Msg("Player will become stuck!!!\n");
			}
#endif
			// actually covered some distance
			interfaces::game_movement_server->mv->SetAbsOrigin(pm.endpos);
			VectorCopy(interfaces::game_movement_server->mv->m_vecVelocity, original_velocity);
			numplanes = 0;
		}

		// If we covered the entire distance, we are done
		//  and can return.
		if (pm.fraction == 1)
		{
			break;		// moved the entire distance
		}

		// Save entity that blocked us (since fraction was < 1.0)
		//  for contact
		// Add it if it's not already in the list!!!
		interfaces::move_helper_server->AddToTouched(pm, interfaces::game_movement_server->mv->m_vecVelocity);

		// If the plane we hit has a high z component in the normal, then
		//  it's probably a floor
		if (pm.plane.normal[2] > 0.7)
		{
			blocked |= 1;		// floor
		}
		// If the plane has a zero z component in the normal, then it's a 
		//  step or wall
		if (abs(pm.plane.normal[2]) < EFFECTIVELY_HORIZONTAL_NORMAL_Z)
		{
			pm.plane.normal[2] = 0;
			blocked |= 2;		// step / wall
		}

		// Reduce amount of m_flFrameTime left by total time left * fraction
		//  that we covered.
		time_left -= time_left * pm.fraction;

		// Did we run out of planes to clip against?
		if (numplanes >= MAX_CLIP_PLANES)
		{
			// this shouldn't really happen
			//  Stop our movement if so.
			VectorCopy(vec3_origin, interfaces::game_movement_server->mv->m_vecVelocity);
			//Con_DPrintf("Too many planes 4\n");

			break;
		}

		// Set up next clipping plane
		VectorCopy(pm.plane.normal, planes[numplanes]);
		numplanes++;

		// modify original_velocity so it parallels all of the clip planes
		//

		// reflect player velocity 
		// Only give this a try for first impact plane because you can get yourself stuck in an acute corner by jumping in place
		//  and pressing forward and nobody was really using this bounce/reflection feature anyway...
		if (numplanes == 1 &&
			interfaces::game_movement_server->player->m_MoveType() == MOVETYPE_WALK &&
			interfaces::game_movement->player->m_hGroundEntity() == NULL)
		{
			for (i = 0; i < numplanes; i++)
			{
				if (planes[i][2] > 0.7)
				{
					// floor or slope
					ClipVelocity(original_velocity, planes[i], new_velocity, 1);
					VectorCopy(new_velocity, original_velocity);
				}
				else
				{
					ClipVelocity(original_velocity, planes[i], new_velocity, 1.0 + interfaces::cvar->FindVar("sv_bounce")->GetFloat() * (1 - interfaces::game_movement_server->player->m_surfaceFriction()));
				}
			}

			VectorCopy(new_velocity, interfaces::game_movement_server->mv->m_vecVelocity);
			VectorCopy(new_velocity, original_velocity);
		}
		else
		{
			for (i = 0; i < numplanes; i++)
			{
				ClipVelocity(original_velocity, planes[i], interfaces::game_movement_server->mv->m_vecVelocity, 1);

				for (j = 0; j < numplanes; j++)
				{
					if (j != i)
					{
						// Are we now moving against this plane?
						if (interfaces::game_movement_server->mv->m_vecVelocity.Dot(planes[j]) < 0)
							break;	// not ok
					}
				}

				if (j == numplanes)  // Didn't have to clip, so we're ok
					break;
			}

			// Did we go all the way through plane set
			if (i != numplanes)
			{	// go along this plane
				// pmove.velocity is set in clipping call, no need to set again.
				;
			}
			else
			{	// go along the crease
				if (numplanes != 2)
				{
					VectorCopy(vec3_origin, interfaces::game_movement_server->mv->m_vecVelocity);
					break;
				}
				CrossProduct(planes[0], planes[1], dir);
				dir.NormalizeInPlace();
				d = dir.Dot(interfaces::game_movement_server->mv->m_vecVelocity);
				VectorScale(dir, d, interfaces::game_movement_server->mv->m_vecVelocity);
			}

			//
			// if original velocity is against the original velocity, stop dead
			// to avoid tiny occilations in sloping corners
			//
			d = interfaces::game_movement_server->mv->m_vecVelocity.Dot(primal_velocity);
			if (d <= 0)
			{
				//Con_DPrintf("Back\n");
				VectorCopy(vec3_origin, interfaces::game_movement_server->mv->m_vecVelocity);
				break;
			}
		}
	}

	if (allFraction == 0)
	{
		VectorCopy(vec3_origin, interfaces::game_movement_server->mv->m_vecVelocity);
	}

	// Check if they slammed into a wall
	float fSlamVol = 0.0f;

	float fLateralStoppingAmount = primal_velocity.Length2D() - interfaces::game_movement_server->mv->m_vecVelocity.Length2D();
	if (fLateralStoppingAmount > PLAYER_MAX_SAFE_FALL_SPEED * 2.0f)
	{
		fSlamVol = 1.0f;
	}
	else if (fLateralStoppingAmount > PLAYER_MAX_SAFE_FALL_SPEED)
	{
		fSlamVol = 0.85f;
	}

	if (fSlamVol > 0.0f)
	{
		interfaces::game_movement_server->PlayerRoughLandingEffects(fSlamVol);
	}

	return blocked;
	//return Hooks::TryPlayerMoveServerOriginal(ecx, ebp, pFirstDest, pFirstTrace);
}

