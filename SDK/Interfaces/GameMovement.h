#pragma once
#include "../Classes/C_BaseEntity.h"
#include "../../Utils/Memory.h"
#include "CTrace.h"
#include "../Classes/BaseHandle.h"

#define MINIMUM_MOVE_FRACTION			0.0001f
#define EFFECTIVELY_HORIZONTAL_NORMAL_Z	0.0001f
#define	STOP_EPSILON		0.1
#define	MAX_CLIP_PLANES		5
#define PLAYER_MAX_SAFE_FALL_SPEED	580 // approx 20 feet
#define GAMEMOVEMENT_DUCK_TIME				1000.0f		// ms
#define GAMEMOVEMENT_JUMP_TIME				510.0f		// ms approx - based on the 21 unit height jump
#define GAMEMOVEMENT_JUMP_HEIGHT			21.0f		// units
#define GAMEMOVEMENT_TIME_TO_UNDUCK			( TIME_TO_UNDUCK * 1000.0f )		// ms
#define GAMEMOVEMENT_TIME_TO_UNDUCK_INV		( GAMEMOVEMENT_DUCK_TIME - GAMEMOVEMENT_TIME_TO_UNDUCK )

inline Vector vec3_origin = { 0.f, 0.f, 0.f };

typedef enum
{
	GROUND = 0,
	STUCK,
	LADDER
} IntervalType_t;

typedef CBaseHandle EntityHandle_t;
#define INVALID_ENTITY_HANDLE INVALID_EHANDLE

enum soundlevel_t
{
	SNDLVL_NONE = 0,

	SNDLVL_20dB = 20,			// rustling leaves
	SNDLVL_25dB = 25,			// whispering
	SNDLVL_30dB = 30,			// library
	SNDLVL_35dB = 35,
	SNDLVL_40dB = 40,
	SNDLVL_45dB = 45,			// refrigerator

	SNDLVL_50dB = 50,	// 3.9	// average home
	SNDLVL_55dB = 55,	// 3.0

	SNDLVL_IDLE = 60,	// 2.0	
	SNDLVL_60dB = 60,	// 2.0	// normal conversation, clothes dryer

	SNDLVL_65dB = 65,	// 1.5	// washing machine, dishwasher
	SNDLVL_STATIC = 66,	// 1.25

	SNDLVL_70dB = 70,	// 1.0	// car, vacuum cleaner, mixer, electric sewing machine

	SNDLVL_NORM = 75,
	SNDLVL_75dB = 75,	// 0.8	// busy traffic

	SNDLVL_80dB = 80,	// 0.7	// mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile
	SNDLVL_TALKING = 80,	// 0.7
	SNDLVL_85dB = 85,	// 0.6	// average factory, electric shaver
	SNDLVL_90dB = 90,	// 0.5	// screaming child, passing motorcycle, convertible ride on frw
	SNDLVL_95dB = 95,
	SNDLVL_100dB = 100,	// 0.4	// subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer
	SNDLVL_105dB = 105,			// helicopter, power mower
	SNDLVL_110dB = 110,			// snowmobile drvrs seat, inboard motorboat, sandblasting
	SNDLVL_120dB = 120,			// auto horn, propeller aircraft
	SNDLVL_130dB = 130,			// air raid siren

	SNDLVL_GUNFIRE = 140,	// 0.27	// THRESHOLD OF PAIN, gunshot, jet engine
	SNDLVL_140dB = 140,	// 0.2

	SNDLVL_150dB = 150,	// 0.2

	SNDLVL_180dB = 180,			// rocket launching

	// NOTE: Valid soundlevel_t values are 0-255.
	//       256-511 are reserved for sounds using goldsrc compatibility attenuation.
};

enum PLAYER_ANIM
{
	PLAYER_IDLE,
	PLAYER_WALK,
	PLAYER_JUMP,
	PLAYER_SUPERJUMP,
	PLAYER_DIE,
	PLAYER_ATTACK1,
	PLAYER_IN_VEHICLE,

	// TF Player animations
	PLAYER_RELOAD,
	PLAYER_START_AIMING,
	PLAYER_LEAVE_AIMING,
};

class CMoveData
{
public:
	bool			m_bFirstRunOfFunctions : 1;
	bool			m_bGameCodeMovedPlayer : 1;
	bool			m_bNoAirControl : 1;

	int	m_nPlayerHandle;	// edict index on server, client entity handle on client

	int				m_nImpulseCommand;	// Impulse command issued.
	Vector			m_vecViewAngles;	// Command view angles (local space)
	Vector			m_vecAbsViewAngles;	// Command view angles (world space)
	int				m_nButtons;			// Attack buttons.
	int				m_nOldButtons;		// From host_client->oldbuttons;
	float			m_flForwardMove;
	float			m_flSideMove;
	float			m_flUpMove;

	float			m_flMaxSpeed;
	float			m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	Vector			m_vecVelocity;		// edict::velocity		// Current movement direction.
	Vector			m_vecAngles;		// edict::angles
	Vector			m_vecOldAngles;

	// Output only
	float			m_outStepHeight;	// how much you climbed this move
	Vector			m_outWishVel;		// This is where you tried 
	Vector			m_outJumpVel;		// This is your jump velocity

	// Movement constraints	(radius 0 means no constraint)
	Vector			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;
	bool			m_bConstraintPastRadius;		///< If no, do no constraining past Radius.  If yes, cap them to SpeedFactor past radius

	void			SetAbsOrigin(const Vector& vec);
	const Vector& GetAbsOrigin() const;

	Vector			m_vecAbsOrigin;		// edict::origin
};

inline const Vector& CMoveData::GetAbsOrigin() const
{
	return m_vecAbsOrigin;
}

inline void CMoveData::SetAbsOrigin(const Vector& vec)
{
	m_vecAbsOrigin = vec;
}

class IMoveHelper
{
public:
	// Call this to set the singleton
	static IMoveHelper* GetSingleton() { return sm_pSingleton; }

	// Methods associated with a particular entity
	virtual	char const* GetName(int handle) const = 0;

	// sets the entity being moved
	virtual void	SetHost(CBaseEntity* host) = 0;

	// Adds the trace result to touch list, if contact is not already in list.
	virtual void	ResetTouchList(void) = 0;
	virtual bool	AddToTouched(const CGameTrace& tr, const Vector& impactvelocity) = 0;
	virtual void	ProcessImpacts(void) = 0;

	// Numbered line printf
	virtual void	Con_NPrintf(int idx, char const* fmt, ...) = 0;

	// These have separate server vs client impementations
	virtual void	StartSound(const Vector& origin, int channel, char const* sample, float volume, soundlevel_t soundlevel, int fFlags, int pitch) = 0;
	virtual void	StartSound(const Vector& origin, const char* soundname) = 0;
	virtual void	PlaybackEventFull(int flags, int clientindex, unsigned short eventindex, float delay, Vector& origin, Vector& angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2) = 0;

	// Apply falling damage to m_pHostPlayer based on m_pHostPlayer->m_flFallVelocity.
	virtual bool	PlayerFallingDamage(void) = 0;

	// Apply falling damage to m_pHostPlayer based on m_pHostPlayer->m_flFallVelocity.
	virtual void	PlayerSetAnimation(PLAYER_ANIM playerAnim) = 0;

	virtual void* GetSurfaceProps(void) = 0;

	virtual bool IsWorldEntity(const CBaseHandle& handle) = 0;

protected:
	// Inherited classes can call this to set the singleton
	static void SetSingleton(IMoveHelper* pMoveHelper) { sm_pSingleton = pMoveHelper; }

	// Clients shouldn't call delete directly
	virtual			~IMoveHelper() {}

	// The global instance
	static IMoveHelper* sm_pSingleton;
};

class IGameMovement
{
public:
	virtual			~IGameMovement(void) {}

	// Process the current movement command
	virtual void	ProcessMovement(CBasePlayer* pPlayer, CMoveData* pMove) = 0;
	virtual void	Reset(void) = 0;
	virtual void	StartTrackPredictionErrors(CBasePlayer* pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(CBasePlayer* pPlayer) = 0;
	virtual void	DiffPrint(char const* fmt, ...) = 0;

	// Allows other parts of the engine to find out the normal and ducked player bbox sizes
	virtual Vector const& GetPlayerMins(bool ducked) const = 0;
	virtual Vector const& GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const& GetPlayerViewOffset(bool ducked) const = 0;

	virtual bool		IsMovingPlayerStuck(void) const = 0;
	virtual CBasePlayer* GetMovingPlayer(void) const = 0;
	virtual void		UnblockPusher(CBasePlayer* pPlayer, CBaseEntity* pPusher) = 0;

	virtual void SetupMovementBounds(CMoveData* pMove) = 0;
};

class CGameMovement : public IGameMovement
{
public:
	CGameMovement(void) {};
	virtual			~CGameMovement(void) {};

	virtual void	ProcessMovement(CBasePlayer* pPlayer, CMoveData* pMove) = 0;
	virtual void	Reset(void) = 0;
	virtual void	StartTrackPredictionErrors(CBasePlayer* pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(CBasePlayer* pPlayer) = 0;
	virtual void	DiffPrint(char const* fmt, ...) = 0;
	virtual const Vector& GetPlayerMins(bool ducked) const = 0;
	virtual const Vector& GetPlayerMaxs(bool ducked) const = 0;
	virtual const Vector& GetPlayerViewOffset(bool ducked) const = 0;
	virtual void SetupMovementBounds(CMoveData* pMove) = 0;
	virtual bool		IsMovingPlayerStuck(void) const = 0;
	virtual CBasePlayer* GetMovingPlayer(void) const = 0;
	virtual void		UnblockPusher(CBasePlayer* pPlayer, CBaseEntity* pPusher) = 0;

	// For sanity checking getting stuck on CMoveData::SetAbsOrigin
	//virtual void			TracePlayerBBox(const Vector& start, const Vector& end, unsigned int fMask, int collisionGroup, trace_t& pm) = 0;

	// wrapper around tracehull to allow tracelistdata optimizations
	void			GameMovementTraceHull(const Vector& start, const Vector& end, const Vector& mins, const Vector& maxs, unsigned int fMask, ITraceFilter* pFilter, trace_t* pTrace);

#define BRUSH_ONLY true
	virtual unsigned int PlayerSolidMask2(bool brushOnly = false, CBasePlayer* testPlayer = NULL) const = 0;	///< returns the solid mask for the given player, so bots can have a more-restrictive set
	CBasePlayer* player;
	CMoveData* GetMoveData() { return mv; }
	// Input/Output for this movement
	CMoveData* mv;

	int				m_nOldWaterLevel;
	float			m_flWaterEntryTime;
	int				m_nOnLadder;

	Vector			m_vecForward;
	Vector			m_vecRight;
	Vector			m_vecUp;
public:

	OFFSET(bool, m_duckUntilOnGround, 0x18AC)
	OFFSET(int, m_iSpeedCropped, 0x1D3)

	bool LadderMove()
	{
		return memory::Call<bool>(this, 41);
	}

	void ReduceTimers()
	{
		memory::Call<void>(this, 52);
	}

	void HandleDuckingSpeedCrop()
	{
		memory::Call<void>(this, 56);
	}

	void FinishUnDuck()
	{
		memory::Call<void>(this, 57);
	}

	void FinishDuck()
	{
		memory::Call<void>(this, 58);
	}

	bool CanUnduck()
	{
		return memory::Call<bool>(this, 59);
	}

	void PlayerRoughLandingEffects(float fvol)
	{
		memory::Call<void>(this, 54, fvol);
	}

	void Duck()
	{
		memory::Call<void>(this, 55);
	}

	void SetGroundEntity(trace_t* pm)
	{
		memory::Call<void>(this, 63, pm);
	}

	unsigned int PlayerSolidMask(bool brushOnly = false, CBasePlayer* testPlayer = NULL)
	{
		return memory::Call<unsigned int>(this, 17, brushOnly, testPlayer);
	}

	void TracePlayerBBox(const Vector& start, const Vector& end, unsigned int fMask, int collisionGroup, trace_t& pm)
	{
		memory::Call<void>(this, 16, std::ref(start), std::ref(end), fMask, collisionGroup, std::ref(pm));
	}

	void CategorizePosition(bool a3 = false)
	{
		using fn = void(__thiscall*)(void*, bool);
		static fn CategorizePositionfn = (fn)memory::PatternScan("client.dll", "55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 FC F3 0F 10 05 ? ? ? ? 56 8B F1 8B");
		CategorizePositionfn(this, a3);
	}

	bool CheckInterval(IntervalType_t type)
	{
		using fn = bool(__thiscall*)(void*, IntervalType_t);
		static fn CategorizePositionfn = (fn)memory::PatternScan("client.dll", "55 8B EC 53 56 8B F1 8B 4D 08 8B 06 8B 90 ? ? ? ? 51 8B CE FF D2 80 3D ? ? ? ? ? 8B D8");
		return CategorizePositionfn(this, type);
	}

	int CheckStuck()
	{
		using fn = int(__thiscall*)(void*);
		static fn CategorizePositionfn = (fn)memory::PatternScan("client.dll", "55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 FC A1 ? ? ? ? 53 56 8B F1 33 C9 89 8D ? ? ? ? 89 8D ? ? ? ? 57");
		return CategorizePositionfn(this);
	}

	void FinishGravity()
	{
		using fn = void(__thiscall*)(void*);
		static fn FinishGravityfn = (fn)memory::PatternScan("client.dll", "8B 51 04 F3 0F 10 82 ? ? ? ? 0F 57 C9 0F 2E C1 9F F6 C4 44 7A 51 F3 0F 10 82 ? ? ? ? 0F 2E C1 9F F6 C4 44 7A 08 F3 0F 10 05 ? ? ? ?");
		FinishGravityfn(this);
	}

	void CheckParameters()
	{
		memory::Call<void>(this, 51);
	}

	void PlaySwimSound()
	{
		memory::Call<void>(this, 62);
	}

	int SetDuckedEyeOffset(float duckFraction)
	{
		using fn = int(__thiscall*)(void*, float);
		static fn SetDuckedEyeOffsetfn = (fn)memory::PatternScan("client.dll", "55 8B EC 83 EC 3C 56 8B F1 8B 46 04");
		return SetDuckedEyeOffsetfn(this, duckFraction);
	}

	void FixPlayerCrouchStuck(bool upward)
	{
		using fn = void(__thiscall*)(void*, bool);
		static fn FixPlayerCrouchStuckfn = (fn)memory::PatternScan("client.dll", "55 8B EC 83 EC 78 A1 ? ? ? ? 33 C5 89 45 FC 33 C0 38 45 08 56 0F 95 C0 57 8B F1 8B 16 8B 92 ? ? ? ? 8D 4D A8 51 8B F8 8B 46 08 05 ? ? ? ? 6A 08 50 8D 45 A4 50 8B CE FF D2");
		FixPlayerCrouchStuckfn(this, upward);
	}

	virtual CBaseHandle	TestPlayerPosition(Vector* start, Vector& pos, int collisionGroup, trace_t& pm)
	{
		return memory::Call<CBaseHandle>(this, 61, start, std::ref(pos), collisionGroup, std::ref(pm));
	}

	void FullNoClipMove(float factor, float maxacceleration)
	{
		using fn = void(__thiscall*)(void*, float, float);
		static fn FullNoClipMovefn = (fn)memory::PatternScan("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 5C A1 ? ? ? ? F3 0F 10 40 ? F3 0F 59 43 ? 56");
		FullNoClipMovefn(this, factor, maxacceleration);
	}

	void FullTossMove()
	{
		memory::Call<void>(this, 35);
	}
	void FullLadderMove()
	{
		memory::Call<void>(this, 36);
	}
	void Eleven()
	{
		memory::Call<void>(this, 37);
	}
	void FullWalkMove()
	{
		memory::Call<void>(this, 30);
	}
	void FullObserverMove()
	{
		using fn = void(__thiscall*)(void*);
		static fn FullObserverMovefn = (fn)memory::PatternScan("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 58 56 57 8B F9 8B 4F 04");
		FullObserverMovefn(this);
	}

};

class CGameMovementServer : public IGameMovement
{
public:
	CGameMovementServer(void) {};
	virtual			~CGameMovementServer(void) {};

	virtual void	ProcessMovement(CBasePlayer* pPlayer, CMoveData* pMove) = 0;
	virtual void	Reset(void) = 0;
	virtual void	StartTrackPredictionErrors(CBasePlayer* pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(CBasePlayer* pPlayer) = 0;
	virtual void	DiffPrint(char const* fmt, ...) = 0;
	virtual const Vector& GetPlayerMins(bool ducked) const = 0;
	virtual const Vector& GetPlayerMaxs(bool ducked) const = 0;
	virtual const Vector& GetPlayerViewOffset(bool ducked) const = 0;
	virtual void SetupMovementBounds(CMoveData* pMove) = 0;
	virtual bool		IsMovingPlayerStuck(void) const = 0;
	virtual CBasePlayer* GetMovingPlayer(void) const = 0;
	virtual void		UnblockPusher(CBasePlayer* pPlayer, CBaseEntity* pPusher) = 0;

	// For sanity checking getting stuck on CMoveData::SetAbsOrigin
	//virtual void			TracePlayerBBox(const Vector& start, const Vector& end, unsigned int fMask, int collisionGroup, trace_t& pm) = 0;

	// wrapper around tracehull to allow tracelistdata optimizations
	void			GameMovementTraceHull(const Vector& start, const Vector& end, const Vector& mins, const Vector& maxs, unsigned int fMask, ITraceFilter* pFilter, trace_t* pTrace);

#define BRUSH_ONLY true
	virtual unsigned int PlayerSolidMask2(bool brushOnly = false, CBasePlayer* testPlayer = NULL) const = 0;	///< returns the solid mask for the given player, so bots can have a more-restrictive set
	CPlayer_Server* player;
	CMoveData* GetMoveData() { return mv; }
	// Input/Output for this movement
	CMoveData* mv;

	int				m_nOldWaterLevel;
	float			m_flWaterEntryTime;
	int				m_nOnLadder;

	Vector			m_vecForward;
	Vector			m_vecRight;
	Vector			m_vecUp;
public:
	OFFSET(bool, m_duckUntilOnGround, 0x28A4)
	OFFSET(int, m_iSpeedCropped, 0x1F4)

	bool LadderMove()
	{
		return memory::Call<bool>(this, 41);
	}

	void ReduceTimers()
	{
		memory::Call<void>(this, 52);
	}

	void HandleDuckingSpeedCrop()
	{
		memory::Call<void>(this, 56);
	}

	void FinishUnDuck()
	{
		memory::Call<void>(this, 57);
	}

	void FinishDuck()
	{
		memory::Call<void>(this, 58);
	}

	bool CanUnduck()
	{
		return memory::Call<bool>(this, 59);
	}

	void PlayerRoughLandingEffects(float fvol)
	{
		memory::Call<void>(this, 54, fvol);
	}

	void Duck()
	{
		memory::Call<void>(this, 55);
	}

	void SetGroundEntity(trace_t* pm)
	{
		memory::Call<void>(this, 63, pm);
	}

	unsigned int PlayerSolidMask(bool brushOnly = false, CBasePlayer* testPlayer = NULL)
	{
		return memory::Call<unsigned int>(this, 17, brushOnly, testPlayer);
	}

	void TracePlayerBBox(const Vector& start, const Vector& end, unsigned int fMask, int collisionGroup, trace_t& pm)
	{
		memory::Call<void>(this, 16, std::ref(start), std::ref(end), fMask, collisionGroup, std::ref(pm));
	}

	void CategorizePosition(bool a3 = false)
	{
		using fn = void(__thiscall*)(void*, bool);
		static fn CategorizePositionfn = (fn)memory::PatternScan("server.dll", "55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 FC F3 0F 10 05 ? ? ? ? 56 8B F1 8B");
		CategorizePositionfn(this, a3);
	}

	bool CheckInterval(IntervalType_t type)
	{
		using fn = bool(__thiscall*)(void*, IntervalType_t);
		static fn CategorizePositionfn = (fn)memory::PatternScan("server.dll", "55 8B EC 56 8B F1 8B 4D 08 8B 06 8B 90 ? ? ? ? 57 51 8B CE FF D2 80 3D ? ? ? ? ? 8B F8");
		return CategorizePositionfn(this, type);
	}

	int CheckStuck()
	{
		using fn = int(__thiscall*)(void*);
		static fn CategorizePositionfn = (fn)memory::PatternScan("server.dll", "55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 FC A1 ? ? ? ? 53 56 57 33 FF 89 BD ? ? ? ? 89 BD");
		return CategorizePositionfn(this);
	}

	void CheckParameters()
	{
		memory::Call<void>(this, 51);
	}

	void PlaySwimSound()
	{
		memory::Call<void>(this, 62);
	}

	void SetDuckedEyeOffset(float duckFraction)
	{
		using fn = void(__thiscall*)(void*, float);
		static fn SetDuckedEyeOffsetfn = (fn)memory::PatternScan("server.dll", "55 8B EC 83 EC 3C 56 8B F1 8B 4E 04");
		SetDuckedEyeOffsetfn(this, duckFraction);
	}

	void FixPlayerCrouchStuck(bool upward)
	{
		using fn = void(__thiscall*)(void*, bool);
		static fn FixPlayerCrouchStuckfn = (fn)memory::PatternScan("server.dll", "55 8B EC 83 EC 78 A1 ? ? ? ? 33 C5 89 45 FC 33 C0 38 45 08 56 0F 95 C0 57 8B F1 8B 16 8B 92 ? ? ? ? 8D 4D A8 51 8B F8 8B 46 08 05 ? ? ? ? 6A 08 50 8D 45 A4 50 8B CE FF D2");
		return FixPlayerCrouchStuckfn(this, upward);
	}

	virtual CBaseHandle	TestPlayerPosition(Vector* start, Vector& pos, int collisionGroup, trace_t& pm)
	{
		return memory::Call<CBaseHandle>(this, 61, start, std::ref(pos), collisionGroup, std::ref(pm));
	}

	void FullNoClipMove(float factor, float maxacceleration)
	{
		using fn = void(__thiscall*)(void*, float, float);
		static fn FullNoClipMovefn = (fn)memory::PatternScan("server.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 5C A1 ? ? ? ? F3 0F 10 40 ? F3 0F 59 43 ? 56 8B F1");
		FullNoClipMovefn(this, factor, maxacceleration);
	}

	void FinishGravity()
	{
		using fn = void(__thiscall*)(void*);
		static fn FinishGravityfn = (fn)memory::PatternScan("server.dll", "8B 51 04 F3 0F 10 82 ? ? ? ? 0F 57 C9 0F 2E C1 9F F6 C4 44 7A 51 F3 0F 10 82 ? ? ? ? 0F 2E C1 9F F6 C4 44 7A 08 F3 0F 10 05 ? ? ? ?");
		FinishGravityfn(this);
	}

	void FullTossMove()
	{
		memory::Call<void>(this, 35);
	}

	void FullLadderMove()
	{
		memory::Call<void>(this, 36);
	}

	void Eleven()
	{
		memory::Call<void>(this, 37);
	}

	void FullWalkMove()
	{
		memory::Call<void>(this, 30);
	}

	void FullObserverMove()
	{
		using fn = void(__thiscall*)(void*);
		static fn FullObserverMovefn = (fn)memory::PatternScan("server.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 58 56 57 8B F9 8B 4F 04");
		FullObserverMovefn(this);
	}
};

inline int ClipVelocity(Vector& in, Vector& normal, Vector& out, float overbounce)
{
	float	backoff;
	float	change;
	float angle;
	int		i, blocked;

	angle = normal[2];

	blocked = 0x00;         // Assume unblocked.
	if (angle > 0)			// If the plane that is blocking us has a positive z component, then assume it's a floor.
		blocked |= 0x01;	// 
	if (!angle)				// If the plane has no Z, it is vertical (wall/step)
		blocked |= 0x02;	// 


	// Determine how far along plane to slide based on incoming direction.
	backoff = DotProduct(in, normal) * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
	}

	// iterate once to make sure we aren't still moving through the plane
	float adjust = DotProduct(out, normal);
	if (adjust < 0.0f)
	{
		// min this against a small number (but no further from zero than -DIST_EPSILON) to account for crossing a plane with a near-parallel normal
		adjust = MIN(adjust, -DIST_EPSILON);
		out -= (normal * adjust);
		//		Msg( "Adjustment = %lf\n", adjust );
	}

	// Return blocking flags.
	return blocked;
}