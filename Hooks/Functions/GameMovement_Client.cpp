#include "../Hooks.h"
#include "../../SDK/Interfaces/GameMovement.h"
#include "../../Utils/Math.h"
#include "../../SDK/Classes/GameMovement_CS.h"

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

void __fastcall Hooks::PlayerMoveClient(void* ecx)
{
	game_movement->CheckParameters();

	// clear output applied velocity
	game_movement->mv->m_outWishVel.Init();
	game_movement->mv->m_outJumpVel.Init();

	move_helper->ResetTouchList();                    // Assume we don't touch anything

	game_movement->ReduceTimers();

	Math::AngleVectors(game_movement->mv->m_vecViewAngles, &game_movement->m_vecForward, &game_movement->m_vecRight, &game_movement->m_vecUp);  // Determine movement angles

	// Always try and unstick us unless we are using a couple of the movement modes
	if (game_movement->player->m_MoveType() != MOVETYPE_NOCLIP &&
		game_movement->player->m_MoveType() != MOVETYPE_NONE &&
		game_movement->player->m_MoveType() != MOVETYPE_ISOMETRIC &&
		game_movement->player->m_MoveType() != MOVETYPE_OBSERVER &&
		!game_movement->player->IsAlive())
	{
		if (game_movement->CheckInterval(STUCK))
		{
			if (game_movement->CheckStuck())
			{
				// Can't move, we're stuck
				return;
			}
		}
	}

	// Now that we are "unstuck", see where we are (game_movement->player->m_nWaterLevel() and type, game_movement->player->GetGroundEntity()).
	if (game_movement->player->m_MoveType() != MOVETYPE_WALK ||
		game_movement->mv->m_bGameCodeMovedPlayer)
	{
		game_movement->CategorizePosition();
	}
	else
	{
		if (game_movement->mv->m_vecVelocity.z > 250.0f)
		{
			game_movement->SetGroundEntity(NULL);
		}
	}

	// Store off the starting water level
	game_movement->m_nOldWaterLevel = (int)game_movement->player->m_nWaterLevel();

	// If we are not on ground, store off how fast we are moving down
	if (game_movement->player->GetGroundEntity() == NULL)
	{
		game_movement->player->m_Local().m_flFallVelocity = -game_movement->mv->m_vecVelocity[2];
	}

	game_movement->m_nOnLadder = 0;

	//game_movement->player->UpdateStepSound(game_movement->player->m_pSurfaceData, mv->GetAbsOrigin(), mv->m_vecVelocity);

	cs_gamemovement.UpdateDuckJumpEyeOffset();
	game_movement->Duck();

	// Don't run ladder code if dead on on a train
	if (!game_movement->player->IsAlive() && !(game_movement->player->m_fFlags() & FL_ONTRAIN))
	{
		// If was not on a ladder now, but was on one before, 
		//  get off of the ladder

		// TODO: this causes lots of weirdness.
		//bool bCheckLadder = CheckInterval( LADDER );
		//if ( bCheckLadder || game_movement->player->m_MoveType() == MOVETYPE_LADDER )
		{
			if (!game_movement->LadderMove() &&
				(game_movement->player->m_MoveType() == MOVETYPE_LADDER))
			{
				// Clear ladder stuff unless player is dead or riding a train
				// It will be reset immediately again next frame if necessary
				game_movement->player->SetMoveType(MOVETYPE_WALK, MOVECOLLIDE_DEFAULT);
				game_movement->player->SetMoveCollide(MOVECOLLIDE_DEFAULT);
			}
		}
	}

	// Handle movement modes.
	switch (game_movement->player->m_MoveType())
	{
	case MOVETYPE_NONE:
		break;

	case MOVETYPE_NOCLIP:
		game_movement->FullNoClipMove(interfaces::cvar->FindVar("sv_noclipspeed")->GetFloat(), interfaces::cvar->FindVar("sv_noclipaccelerate")->GetFloat());
		break;

	case MOVETYPE_FLY:
	case MOVETYPE_FLYGRAVITY:
		game_movement->FullTossMove();
		break;

	case MOVETYPE_LADDER:
		game_movement->FullLadderMove();
		break;

	case MOVETYPE_WALK:
		game_movement->FullWalkMove();
		break;

	case MOVETYPE_ISOMETRIC:
		//IsometricMove();
		// Could also try:  FullTossMove();
		game_movement->FullWalkMove();
		break;

	case MOVETYPE_OBSERVER:
		game_movement->FullObserverMove(); // clips against world&players
		break;
	case MOVETYPE_CUSTOM:
		game_movement->Eleven();		   //specific movetype - idk i don't play l4d2
		break;
	default:
		//DevMsg(1, "Bogus pmove player movetype %i on (%i) 0=cl 1=sv\n", game_movement->player->m_MoveType(), game_movement->player->IsServer());
		break;
	}

	//Hooks::PlayerMoveOriginal(ecx);
}

void __fastcall Hooks::DuckClientHook(void* ecx)
{
	static float m_fTimeLastUnducked = 0.f;
	// Fix taken from zblock for rapid crouch/stand not showing stand on other clients
	if (!Config::Movements::bCSGameMovement || !engine->IsDedicatedServer())
		return Hooks::DuckClientOriginal(ecx);

	if (game_movement->player->m_fFlags() & FL_ONGROUND)
	{
		// if prevent crouch
		if (!(game_movement->mv->m_nButtons & IN_DUCK) && (game_movement->mv->m_nOldButtons & IN_DUCK))
		{
			// Player has released crouch and moving to standing
			m_fTimeLastUnducked = interfaces::globals->curtime;
		}
		else if ((game_movement->mv->m_nButtons & IN_DUCK) && !(game_movement->mv->m_nOldButtons & IN_DUCK))
		{
			// Crouch from standing
			if ((game_movement->player->m_fFlags() & FL_DUCKING)
				&& (m_fTimeLastUnducked > (interfaces::globals->curtime - 0.f)))
			{
				// if the server thinks the player is still crouched
				// AND the time the player started to stand (from being ducked) was less than 300ms ago
				// prevent the player from ducking again
				game_movement->mv->m_nButtons &= ~IN_DUCK;
			}
		}
	}

	int buttonsChanged = (game_movement->mv->m_nOldButtons ^ game_movement->mv->m_nButtons);	// These buttons have changed this frame
	int buttonsPressed = buttonsChanged & game_movement->mv->m_nButtons;			// The changed ones still down are "pressed"
	int buttonsReleased = buttonsChanged & game_movement->mv->m_nOldButtons;		// The changed ones which were previously down are "released"

	// Check to see if we are in the air.
	bool bInAir = game_movement->player->GetGroundEntity() == NULL && game_movement->player->m_MoveType() != MOVETYPE_LADDER;

	if (game_movement->mv->m_nButtons & IN_DUCK)
	{
		game_movement->mv->m_nOldButtons |= IN_DUCK;
	}
	else
	{
		game_movement->mv->m_nOldButtons &= ~IN_DUCK;
	}

	if (!game_movement->player->IsAlive())
	{
		// Unduck
		if (game_movement->player->m_fFlags() & FL_DUCKING)
		{
			game_movement->FinishUnDuck();
		}
		return;
	}

	cs_gamemovement.HandleDuckingSpeedCrop();

	if (game_movement->m_duckUntilOnGround())
	{
		if (!bInAir)
		{
			game_movement->m_duckUntilOnGround() = false;
			if (game_movement->CanUnduck())
			{
				game_movement->FinishUnDuck();
			}
			return;
		}
		else
		{
			if (game_movement->mv->m_vecVelocity.z > 0.0f)
				return;

			// Check if we can un-duck.  We want to unduck if we have space for the standing hull, and
			// if it is less than 2 inches off the ground.
			trace_t trace;
			Vector newOrigin;
			Vector groundCheck;

			VectorCopy(game_movement->mv->GetAbsOrigin(), newOrigin);
			Vector hullSizeNormal = VEC_HULL_MAX_SCALED(game_movement->player) - VEC_HULL_MIN_SCALED(game_movement->player);
			Vector hullSizeCrouch = VEC_DUCK_HULL_MAX_SCALED(game_movement->player) - VEC_DUCK_HULL_MIN_SCALED(game_movement->player);
			newOrigin -= (hullSizeNormal - hullSizeCrouch);
			groundCheck = newOrigin;
			groundCheck.z -= game_movement->player->m_flStepSize();

			UTIL_TraceHull(newOrigin, groundCheck, VEC_HULL_MIN_SCALED(game_movement->player), VEC_HULL_MAX_SCALED(game_movement->player), MASK_PLAYERSOLID, game_movement->player, COLLISION_GROUP_PLAYER_MOVEMENT, &trace, false);

			if (trace.startsolid || trace.fraction == 1.0f)
				return; // Can't even stand up, or there's no ground underneath us

			game_movement->m_duckUntilOnGround() = false;
			if (game_movement->CanUnduck())
			{
				game_movement->FinishUnDuck();
			}
			return;
		}
	}

	// Holding duck, in process of ducking or fully ducked?
	if ((game_movement->mv->m_nButtons & IN_DUCK) || (game_movement->player->m_Local().m_bDucking) || (game_movement->player->m_fFlags() & FL_DUCKING))
	{
		if (game_movement->mv->m_nButtons & IN_DUCK)
		{
			bool alreadyDucked = (game_movement->player->m_fFlags() & FL_DUCKING) ? true : false;

			if ((buttonsPressed & IN_DUCK) && !(game_movement->player->m_fFlags() & FL_DUCKING))
			{
				// Use 1 second so super long jump will work
				game_movement->player->m_Local().m_flDucktime = 1000;
				game_movement->player->m_Local().m_bDucking = true;
			}

			float duckmilliseconds = MAX(0.0f, 1000.0f - (float)game_movement->player->m_Local().m_flDucktime);
			float duckseconds = duckmilliseconds / 1000.0f;

			//time = MAX( 0.0, ( 1.0 - (float)player->m_Local.m_flDucktime / 1000.0 ) );

			if (game_movement->player->m_Local().m_bDucking)
			{
				//if (!(game_movement->player->m_fFlags() & FL_ANIMDUCKING))
				//	game_movement->player->AddFlag(FL_ANIMDUCKING);

				// Finish ducking immediately if duck time is over or not on ground
				if ((duckseconds > TIME_TO_DUCK) ||
					(game_movement->player->GetGroundEntity() == NULL) ||
					alreadyDucked)
				{
					game_movement->FinishDuck();
				}
				else
				{
					// Calc parametric time
					float duckFraction = SimpleSpline(duckseconds / TIME_TO_DUCK);
					game_movement->SetDuckedEyeOffset(duckFraction);
				}
			}
		}
		else
		{
			// Try to unduck unless automovement is not allowed
			// NOTE: When not onground, you can always unduck
			if (game_movement->player->m_Local().m_bAllowAutoMovement || game_movement->player->GetGroundEntity() == NULL)
			{
				if ((buttonsReleased & IN_DUCK) && (game_movement->player->m_fFlags() & FL_DUCKING))
				{
					// Use 1 second so super long jump will work
					game_movement->player->m_Local().m_flDucktime = 1000;
					game_movement->player->m_Local().m_bDucking = true;  // or unducking
				}

				float duckmilliseconds = MAX(0.0f, 1000.0f - (float)game_movement->player->m_Local().m_flDucktime);
				float duckseconds = duckmilliseconds / 1000.0f;

				if (game_movement->CanUnduck())
				{
					if (game_movement->player->m_Local().m_bDucking ||
						game_movement->player->m_Local().m_bDucked) // or unducking
					{
						//if (game_movement->player->m_fFlags() & FL_ANIMDUCKING)
						//	game_movement->player->RemoveFlag(FL_ANIMDUCKING);

						// Finish ducking immediately if duck time is over or not on ground
						if ((duckseconds > TIME_TO_UNDUCK) ||
							(game_movement->player->GetGroundEntity() == NULL))
						{
							game_movement->FinishUnDuck();
						}
						else
						{
							// Calc parametric time
							float duckFraction = SimpleSpline(1.0f - (duckseconds / TIME_TO_UNDUCK));
							game_movement->SetDuckedEyeOffset(duckFraction);
						}
					}
				}
				else
				{
					// Still under something where we can't unduck, so make sure we reset this timer so
					//  that we'll unduck once we exit the tunnel, etc.
					game_movement->player->m_Local().m_flDucktime = 1000;
				}
			}
		}
	}

	//Hooks::DuckClientOriginal(ecx);
}

bool __fastcall Hooks::CheckJumpButton(void* ecx)
{
	if (!Config::Movements::bCSGameMovement || !engine->IsDedicatedServer())
		return Hooks::CheckJumpButtonOriginal(ecx);

	if (!game_movement->player->IsAlive())
	{
		game_movement->mv->m_nOldButtons |= IN_JUMP;	// don't jump again until released
		return false;
	}

	// See if we are waterjumping.  If so, decrement count and return.
	if (game_movement->player->m_flWaterJumpTime())
	{
		game_movement->player->m_flWaterJumpTime() -= interfaces::globals->frametime;
		if (game_movement->player->m_flWaterJumpTime() < 0)
			game_movement->player->m_flWaterJumpTime() = 0;

		return false;
	}

	// If we are in the water most of the way...
	if (game_movement->player->m_nWaterLevel() >= 2)
	{
		// swimming, not jumping
		game_movement->SetGroundEntity(NULL);

		if (game_movement->player->GetWaterType() == CONTENTS_WATER)    // We move up a certain amount
			game_movement->mv->m_vecVelocity[2] = 100;
		else if (game_movement->player->GetWaterType() == CONTENTS_SLIME)
			game_movement->mv->m_vecVelocity[2] = 80;

		// play swiming sound
		if (game_movement->player->m_flSwimSoundTime() <= 0)
		{
			// Don't play sound again for 1 second
			game_movement->player->m_flSwimSoundTime() = 1000;
			game_movement->PlaySwimSound();
		}

		return false;
	}

	// No more effect
	if (game_movement->player->GetGroundEntity() == NULL)
	{
		game_movement->mv->m_nOldButtons |= IN_JUMP;
		return false;		// in air, so no effect
	}

	if (game_movement->mv->m_nOldButtons & IN_JUMP)
		return false;		// don't pogo stick

	//if (!sv_enablebunnyhopping.GetBool())
	//{
	//	PreventBunnyJumping();
	//}

	// In the air now.
	game_movement->SetGroundEntity(NULL);

	//game_movement->player->PlayStepSound((Vector&)mv->GetAbsOrigin(), player->m_pSurfaceData, 1.0, true);

	//MoveHelper()->PlayerSetAnimation( PLAYER_JUMP );
	//game_movement->player->DoAnimationEvent(PLAYERANIMEVENT_JUMP);

	float flGroundFactor = 1.0f;
	//if (game_movement->player->m_pSurfaceData())
	//{
	//	flGroundFactor = game_movement->player->m_pSurfaceData()->game.jumpFactor;
	//}

	// if we weren't ducking, bots and hostages do a crouchjump programatically
	if ((!game_movement->player) && !(game_movement->mv->m_nButtons & IN_DUCK))
	{
		game_movement->player->m_duckUntilOnGround() = true;
		game_movement->FinishDuck();
	}

	// Acclerate upward
	// If we are ducking...
	float startz = game_movement->mv->m_vecVelocity[2];
	if (game_movement->player->m_duckUntilOnGround() || (game_movement->player->m_Local().m_bDucking) || (game_movement->player->m_fFlags() & FL_DUCKING))
	{
		// d = 0.5 * g * t^2		- distance traveled with linear accel
		// t = sqrt(2.0 * 45 / g)	- how long to fall 45 units
		// v = g * t				- velocity at the end (just invert it to jump up that high)
		// v = g * sqrt(2.0 * 45 / g )
		// v^2 = g * g * 2.0 * 45 / g
		// v = sqrt( g * 2.0 * 45 )

		game_movement->mv->m_vecVelocity[2] = flGroundFactor * sqrt(2 * 800 * 57.0);  // 2 * gravity * height
	}
	else
	{
		game_movement->mv->m_vecVelocity[2] += flGroundFactor * sqrt(2 * 800 * 57.0);  // 2 * gravity * height
	}

	if (game_movement->player->m_flStamina() > 0)
	{
		float flRatio;

		flRatio = (STAMINA_MAX - ((game_movement->player->m_flStamina() / 1000.0) * STAMINA_RECOVER_RATE)) / STAMINA_MAX;

		game_movement->mv->m_vecVelocity[2] *= flRatio;
	}

	game_movement->player->m_flStamina() = (STAMINA_COST_JUMP / STAMINA_RECOVER_RATE) * 1000.0;

	game_movement->FinishGravity();

	game_movement->mv->m_outWishVel.z += game_movement->mv->m_vecVelocity[2] - startz;
	game_movement->mv->m_outStepHeight += 0.1f;

	// Flag that we jumped.
	game_movement->mv->m_nOldButtons |= IN_JUMP;	// don't jump again until released
	return true;

	//return Hooks::CheckJumpButtonOriginal(ecx);
}

int __fastcall Hooks::TryPlayerMoveClient(void* ecx, void* ebp, Vector* pFirstDest, trace_t* pFirstTrace)
{
	if (!Config::Movements::bCSGameMovement || !engine->IsDedicatedServer())
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
			game_movement->player->GetGroundEntity() == NULL)
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


