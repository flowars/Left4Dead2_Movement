#include "../Hooks.h"
#include "../../SDK/Interfaces/GameMovement.h"
#include "../../Utils/Math.h"
#include "../../SDK/Interfaces/CSGO_GameMovement.h"

using namespace interfaces;

void __stdcall Hooks::CategorizePositionHook(bool a3)
{
	Hooks::CategorizePositionOriginal(game_movement, a3);
}

void __fastcall Hooks::ReduceTimersClient(void* ecx)
{
	//if (game_movement->player->m_flStamina > 0)
	//{
	//	game_movement->player->m_flStamina -= globals->frametime * 60.f;		//60.f - sv_staminarecoveryrate

	//	if (game_movement->player->m_flStamina < 0)
	//	{
	//		game_movement->player->m_flStamina = 0;
	//	}
	//}
	Hooks::ReduceTimersClientOriginal(game_movement);
}

void __fastcall Hooks::DuckClientHook(void* ecx)
{
	//static bool m_duckUntilOnGround_c;

	//const bool playerTouchingGround = game_movement->player->m_hGroundEntity() != NULL;

	//// Check to see if we are in the air.
	//const bool bInAir = !playerTouchingGround && game_movement->player->m_MoveType() != MOVETYPE_LADDER;

	//if (game_movement->mv->m_nButtons & IN_DUCK)
	//{
	//	game_movement->mv->m_nOldButtons |= IN_DUCK;
	//}
	//else
	//{
	//	game_movement->mv->m_nOldButtons &= ~IN_DUCK;
	//}

	//// Dead players don't duck.
	//if (!game_movement->player->IsAlive() && !game_movement->player->IsObserver())
	//{
	//	if (game_movement->player->m_fFlags() & FL_DUCKING)
	//	{
	//		g_GameMovement_client.FinishUnDuck();
	//	}

	//	return;
	//}

	//if (m_duckUntilOnGround_c)
	//{
	//	// This code handles the case where a bot is jumping; they
	//	// automatically crouch jump, and we want to decide if they are
	//	// ready to un-duck here.

	//	// TODO: Should we move this code into the bot movement logic
	//	//       instead?

	//	// $$$REI There still seems to be a way to end up in this state if the bot
	//	//        was crouch-jumping at the end of the round; I haven't found where
	//	//        his flags are getting reset.  Will fix later, and just reset the
	//	//        inconsistent state here.  Next tick the bot will behave normally.
	//	// Assert( player->GetFlags() & FL_DUCKING );
	//	if ((game_movement->player->m_fFlags() & FL_DUCKING) == 0)
	//	{
	//		m_duckUntilOnGround_c = false;
	//		return;
	//	}

	//	// If we have landed, we are done with 'duck until on ground'.
	//	if (!bInAir)
	//	{
	//		m_duckUntilOnGround_c = false;

	//		// Stop crouching if possible
	//		if (g_GameMovement_client.CanUnduck())
	//		{
	//			g_GameMovement_client.FinishUnDuck();
	//		}

	//		return;
	//	}

	//	// Otherwise we are still in the air.
	//	// Try to un-duck just as we land, for better animation and movement.

	//	// If we're still going up, we aren't about to land.  Early-out.
	//	if (game_movement->mv->m_vecVelocity.z > 0.0f)
	//		return;

	//	// Check if we are close enough to the ground and that there is room to un-duck.
	//	trace_t trace;
	//	Vector newOrigin;
	//	Vector groundCheck;

	//	VectorCopy(game_movement->mv->GetAbsOrigin(), newOrigin);
	//	Vector hullSizeNormal = VEC_HULL_MAX - VEC_HULL_MIN;
	//	Vector hullSizeCrouch = VEC_DUCK_HULL_MAX - VEC_DUCK_HULL_MIN;
	//	newOrigin -= (hullSizeNormal - hullSizeCrouch);
	//	groundCheck = newOrigin;
	//	groundCheck.z -= game_movement->player->m_flStepSize();

	//	UTIL_TraceHull(newOrigin, groundCheck, VEC_HULL_MIN, VEC_HULL_MAX, game_movement->PlayerSolidMask(), game_movement->player, COLLISION_GROUP_PLAYER_MOVEMENT, &trace);
	//	if (trace.startsolid			// No room to unduck.
	//		|| trace.fraction == 1.0f	// We are still in the air
	//		)
	//		return;

	//	// Success!  We can un-duck.  Remove "un-duck when possible" flag.
	//	m_duckUntilOnGround_c = false;

	//	// Theoretically CanUnduck() should always succeed here since we just did a hull trace.
	//	// REI: But the hulltrace in CanUnduck() looks slightly different than this one; it uses
	//	//         newOrigin = mv->GetAbsOrigin() + -0.5f * ( hullSizeNormal - hullSizeCrouch )
	//	//      and traces from mv->GetAbsOrigin() to newOrigin instead of from newOrigin to a
	//	//      step away.
	//	if (g_GameMovement_client.CanUnduck())
	//	{
	//		g_GameMovement_client.FinishUnDuck();
	//	}

	//	return;
	//}

	//// Reduce duck-spam penalty over time
	//client::m_flDuckSpeed = Approach(CS_PLAYER_DUCK_SPEED_IDEAL, client::m_flDuckSpeed, globals->interval_per_tick * 3.0f);

	//// Use the last-known position of full crouch speed to restore crouch speed as a function of physical player position.
	//// The goal is that moving a sufficient distance should reset crouch speed in an intuitive manner.
	//if (client::m_flDuckSpeed >= CS_PLAYER_DUCK_SPEED_IDEAL)
	//{
	//	client::m_vecLastPositionAtFullCrouchSpeed = game_movement->player->GetAbsOrigin().AsVector2D();
	//}
	//else if (client::m_flDuckAmount <= 0 || client::m_flDuckAmount >= 1)
	//{
	//	//debugoverlay->AddLineOverlay( player->m_vecLastPositionAtFullCrouchSpeed, player->GetAbsOrigin(), 255,0,0, true, 0.1f );
	//	//debugoverlay->AddTextOverlay( player->GetAbsOrigin(), 0.1f, "%f", player->m_flDuckSpeed );

	//	float flDistToLastPositionAtFullCrouchSpeed = client::m_vecLastPositionAtFullCrouchSpeed.DistToSqr(game_movement->player->GetAbsOrigin().AsVector2D());

	//	// if we're sufficiently far from the last full crouch speed location, we can safely restore crouch speed faster.
	//	if (flDistToLastPositionAtFullCrouchSpeed > (64 * 64))
	//	{
	//		client::m_flDuckSpeed = Approach(CS_PLAYER_DUCK_SPEED_IDEAL, client::m_flDuckSpeed, globals->interval_per_tick * 6.0f);
	//	}
	//}

	//bool duckButtonHeld = (game_movement->mv->m_nButtons & IN_DUCK) != 0;

	//if (!duckButtonHeld && client::m_flDuckAmount > 0)
	//{
	//	// Not sure if this is the appropriate use of this flag. It seems odd to have a dedicated variable that effectively means crouch-is-not-zero-or-one.

	//	// When the round restarts with the player in the ducked state, they can get stuck crouched.
	//	// To prevent this, I'm setting the "duck-in-progress" bool (m_bDucking) to true if
	//	// the player is ever in the state of NOT holding the duck button but is still ducked.
	//	client::m_bDucking = true;
	//}
	//else if (duckButtonHeld && client::m_flDuckAmount < 1)
	//{
	//	// or if the player IS holding the duck button but isn't yet fully ducked.
	//	client::m_bDucking = true;
	//}

	//// Handle animating into the ducking pose.
	//if (duckButtonHeld && client::m_bDucking)
	//{
	//	//Assert(!player->m_Local.m_bDucked);

	//	// ducking is always a little slower than unducking
	//	float duckSpeed = client::m_flDuckSpeed * 0.8f;

	//	client::m_flDuckAmount = Approach(1.0f, client::m_flDuckAmount, globals->interval_per_tick * duckSpeed);

	//	// Finish ducking immediately if duck time is over or not on ground
	//	if (client::m_flDuckAmount >= 1.0f || !playerTouchingGround)
	//	{
	//		g_GameMovement_client.FinishDuck();
	//	}
	//	else
	//	{
	//		game_movement->SetDuckedEyeOffset(client::m_flDuckAmount);
	//	}

	//	// REI: For some reason we don't set this flag immediately, but wait until you have ducked a little bit.  Investigate?
	//	if (client::m_flDuckAmount >= 0.1f && !(game_movement->player->m_fFlags() & FL_ANIMDUCKING))
	//	{
	//		game_movement->player->AddFlag(FL_ANIMDUCKING);
	//	}
	//}

	//// Handle animating out of ducking pose.
	//if (!duckButtonHeld && client::m_bDucking
	//	// Try to unduck unless automovement is not allowed
	//	// NOTE: When not onground, you can always unduck
	//	// REI: Cloned behavior from old code, not sure when m_bAllowAutomovement is used?
	//	&& (client::m_bAllowAutoMovement || !playerTouchingGround))
	//{
	//	if (g_GameMovement_client.CanUnduck())
	//	{
	//		// Always unduck at at least 1.5 to prevent advantageous semi-ducked positions
	//		float duckSpeed = MAX(1.5f, client::m_flDuckSpeed);

	//		client::m_flDuckAmount = Approach(0.0f, client::m_flDuckAmount, globals->interval_per_tick * duckSpeed);
	//		client::m_bDucked = false;

	//		if (client::m_flDuckAmount <= 0.0f || !playerTouchingGround)
	//		{
	//			g_GameMovement_client.FinishUnDuck();
	//		}
	//		else
	//		{
	//			game_movement->SetDuckedEyeOffset(client::m_flDuckAmount);
	//		}

	//		// Remove the ducked flags if we're not fully ducked anymore.
	//		// REI: This is inconsistent with the documentation for these flags, but I'm not sure why the code
	//		//      is doing this.  It does mean you lose your ducking accuracy bonus very early in the un-duck,
	//		//      which is certainly important.
	//		if (client::m_flDuckAmount <= 0.75f && game_movement->player->m_fFlags() & (FL_ANIMDUCKING | FL_DUCKING))
	//		{
	//			game_movement->player->RemoveFlag(FL_ANIMDUCKING | FL_DUCKING);
	//		}
	//	}
	//	else
	//	{
	//		// Reset to fully-ducked as we went under something we can't un-duck from.
	//		// We'll try again once the player has moved out of the obstructing obstacle.
	//		client::m_flDuckAmount = 1.0f;
	//		client::m_bDucked = true;
	//		client::m_bDucking = false;
	//		game_movement->player->AddFlag(FL_ANIMDUCKING | FL_DUCKING);

	//		game_movement->SetDuckedEyeOffset(client::m_flDuckAmount);
	//	}
	//}

	//// REI: I think I've fixed all cases of this happening.  Leaving it in for now.  $$$REI remove this after testing shows it never happening again.
	//if (client::m_flDuckAmount <= 0 && (game_movement->player->m_fFlags() & FL_ANIMDUCKING))
	//{
	//	//AssertMsg1(false, "Clearing FL_ANIMDUCKING flag on player %s to prevent crab-walk.  Please let Ryan know if you hit this.", player->GetPlayerName());
	//	game_movement->player->RemoveFlag(FL_ANIMDUCKING);
	//}

	////if (IsPreCrouchUpdateDemo())
	////{
	////	// compatibility for old demos using the old crouch values
	////	if (player->m_Local.m_nDuckTimeMsecs)
	////	{
	////		player->AddFlag(FL_ANIMDUCKING | FL_DUCKING);
	////	}
	////	else
	////	{
	////		player->RemoveFlag(FL_ANIMDUCKING | FL_DUCKING);
	////	}
	////	const float CS_DUCK_TIME_MSECS = 150.0f;
	////	int millisecondsDucked = MAX(0, CS_DUCK_TIME_MSECS - player->m_Local.m_nDuckTimeMsecs);
	////	player->m_flDuckAmount = (float)millisecondsDucked / (float)CS_DUCK_TIME_MSECS;
	////	SetDuckedEyeOffset(player->m_flDuckAmount);
	////}

	//g_GameMovement_client.HandleDuckingSpeedCrop(client::m_flDuckAmount);
	Hooks::DuckClientOriginal(ecx);
}

int __fastcall Hooks::TryPlayerMoveClient(void* ecx, void* ebp, Vector* pFirstDest, trace_t* pFirstTrace)
{
	if (!Config::Movements::bTryPlayerMove || !engine->IsDedicatedServer())
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

	VectorCopy(game_movement->mv->m_vecVelocity, original_velocity);  // Store original velocity
	VectorCopy(game_movement->mv->m_vecVelocity, primal_velocity);

	allFraction = 0;
	time_left = globals->interval_per_tick;   // Total time for this movement operation.

	new_velocity.Init();
	for (bumpcount = 0; bumpcount < numbumps; bumpcount++)
	{
		if (game_movement->mv->m_vecVelocity.Length() == 0.0)
			break;

		// Assume we can move all the way from the current origin to the
		//  end point.
		VectorMA(game_movement->mv->GetAbsOrigin(), time_left, game_movement->mv->m_vecVelocity, end);

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
				TracePlayerBboxCSGO(game_movement->mv->GetAbsOrigin(), game_movement->mv->GetAbsOrigin(), MASK_PLAYERSOLID, COLLISION_GROUP_PLAYER_MOVEMENT, foo);
				if (foo.startsolid || foo.fraction != 1.0f)
				{
					Msg("bah\n");
				}
#endif
				game_movement->TracePlayerBBox(game_movement->mv->GetAbsOrigin(), end, game_movement->PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, pm);
			}
		}
		else
		{
			game_movement->TracePlayerBBox(game_movement->mv->GetAbsOrigin(), end, game_movement->PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, pm);
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
			VectorCopy(vec3_origin, game_movement->mv->m_vecVelocity);
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
				game_movement->TracePlayerBBox(pm.endpos, pm.endpos, game_movement->PlayerSolidMask(), COLLISION_GROUP_PLAYER_MOVEMENT, stuck);
				if (stuck.startsolid || stuck.fraction != 1.0f)
				{
					//Msg( "Player will become stuck!!!\n" );
					VectorCopy(vec3_origin, game_movement->mv->m_vecVelocity);
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
			game_movement->mv->SetAbsOrigin(pm.endpos);
			VectorCopy(game_movement->mv->m_vecVelocity, original_velocity);
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
		move_helper->AddToTouched(pm, game_movement->mv->m_vecVelocity);

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
			VectorCopy(vec3_origin, game_movement->mv->m_vecVelocity);
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
			game_movement->player->m_MoveType() == MOVETYPE_WALK &&
			game_movement->player->m_hGroundEntity() == NULL)
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
					ClipVelocity(original_velocity, planes[i], new_velocity, 1.0 + cvar->FindVar("sv_bounce")->GetFloat() * (1 - game_movement->player->m_surfaceFriction()));
				}
			}

			VectorCopy(new_velocity, game_movement->mv->m_vecVelocity);
			VectorCopy(new_velocity, original_velocity);
		}
		else
		{
			for (i = 0; i < numplanes; i++)
			{
				ClipVelocity(original_velocity, planes[i], game_movement->mv->m_vecVelocity, 1);

				for (j = 0; j < numplanes; j++)
				{
					if (j != i)
					{
						// Are we now moving against this plane?
						if (game_movement->mv->m_vecVelocity.Dot(planes[j]) < 0)
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
					VectorCopy(vec3_origin, game_movement->mv->m_vecVelocity);
					break;
				}
				CrossProduct(planes[0], planes[1], dir);
				dir.NormalizeInPlace();
				d = dir.Dot(game_movement->mv->m_vecVelocity);
				VectorScale(dir, d, game_movement->mv->m_vecVelocity);
			}

			//
			// if original velocity is against the original velocity, stop dead
			// to avoid tiny occilations in sloping corners
			//
			d = game_movement->mv->m_vecVelocity.Dot(primal_velocity);
			if (d <= 0)
			{
				//Con_DPrintf("Back\n");
				VectorCopy(vec3_origin, game_movement->mv->m_vecVelocity);
				break;
			}
		}
	}

	if (allFraction == 0)
	{
		VectorCopy(vec3_origin, game_movement->mv->m_vecVelocity);
	}

	// Check if they slammed into a wall
	float fSlamVol = 0.0f;

	float fLateralStoppingAmount = primal_velocity.Length2D() - game_movement->mv->m_vecVelocity.Length2D();
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
		game_movement->PlayerRoughLandingEffects(fSlamVol);
	}

	return blocked;
}


