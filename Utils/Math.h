#pragma once
#include <cmath>
#include "directxmath.h"
#include "../SDK/Classes/Vector.h"

#define M_PI		3.14159265358979323846
#define M_PI_R		0.01745329251
#define M_PI_F		((float)(M_PI))

// MOVEMENT INFO
enum
{
	PITCH = 0,	// up / down
	YAW,		// left / right
	ROLL		// fall over
};

namespace Math
{
	inline auto Deg2Rad(float Degrees) { return Degrees * (M_PI_F / 180.0f); }
	inline auto Rad2Deg(float Radians) { return Radians * (180.0f / M_PI_F); }

	inline float NormalizeYaw(float Yaw) {
		if (!std::isfinite(Yaw))
			return 0.0f;

		if (Yaw >= -180.f && Yaw <= 180.f)
			return Yaw;

		const float rot = std::round(std::abs(Yaw / 360.f));

		Yaw = (Yaw < 0.f) ? Yaw + (360.f * rot) : Yaw - (360.f * rot);
		return Yaw;
	}

	inline void VectorAngles(const Vector& forward, Vector& angles)
	{
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	inline void SinCos(float r, float* s, float* c) {
		*s = sin(r);
		*c = cos(r);
	}

	inline void AngleVectors(const Vector& angles, Vector* forward) {
		float sp, sy, cp, cy;
		SinCos(Deg2Rad(angles[0]), &sp, &cp);
		SinCos(Deg2Rad(angles[1]), &sy, &cy);

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	inline void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(Deg2Rad(angles[YAW]), &sy, &cy);
		SinCos(Deg2Rad(angles[PITCH]), &sp, &cp);
		SinCos(Deg2Rad(angles[ROLL]), &sr, &cr);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
			right->y = (-1 * sr * sp * sy + -1 * cr * cy);
			right->z = -1 * sr * cp;
		}

		if (up)
		{
			up->x = (cr * sp * cy + -sr * -sy);
			up->y = (cr * sp * sy + -sr * cy);
			up->z = cr * cp;
		}
	}
}