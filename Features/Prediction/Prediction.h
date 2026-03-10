#pragma once

#include "../../Core/Interfaces.h"
#include "../../SDK/Classes/Vector.h"
#include "../../Core/Globals.h"
#include "../Utils/Memory.h"


namespace Prediction_backup
{
	inline Vector vec_origin;
	inline Vector vec_velocity;
	inline int flags;
}

namespace Prediction
{
	inline void BackupPrediction()
	{
		Prediction_backup::vec_origin = l4d2::local->m_vecOrigin();
		Prediction_backup::vec_velocity = l4d2::local->m_vecVelocity();
		Prediction_backup::flags = l4d2::local->m_fFlags();
	}

	void Update();
	void Begin(CUserCmd* cmd);
	void Finish();
	void RestoreEntityToPredictedFrame(int i);
	int GetTickBase(CUserCmd* cmd);

	inline CMoveData mv;
	inline unsigned int* Prediction_RandomSeed;
	inline bool InPrediction;

	inline float m_flOldCurTime;
	inline float m_flOldFrameTime;
	inline int m_nOldTickCount;
}