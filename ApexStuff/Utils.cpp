#include "common.h"


float Distance(Vec3 EntityPos, Vec3 PlayerPos)
{
	float Dist = sqrt((PlayerPos.x - EntityPos.x) * (PlayerPos.x - EntityPos.x)
		+ (PlayerPos.y - EntityPos.y) * (PlayerPos.y - EntityPos.y)
		+ (PlayerPos.z - EntityPos.z) * (PlayerPos.z - EntityPos.z));
	return Dist * 0.01905f;
}

//int Distance(Vec3 first, Vec3 second)
//{
//	return (int)sqrtf(((second.x - first.x) * (second.x - first.x)) + ((second.y - first.y) * (second.y - first.y)) + ((second.z - first.z) * (second.z - first.z)));
//}

Vec3 Cross(Vec3 a, Vec3 b)
{
	Vec3 product;
	product.x = (a.y * b.z) - (a.z * b.y);
	product.y = (a.z * b.x) - (a.x * b.z);
	product.z = (a.x * b.y) - (a.y * b.x);
	return product;
}

float Dot(Vec3 left, Vec3 right)
{
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}


void GetTextSize(const char* str, LPSIZE size)
{
	RECT rect = { 0,0,0,0 };
	if (pFont)
	{
		pFont->DrawText(NULL, str, strlen(str), &rect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	}

	size->cx = rect.right - rect.left;
	size->cy = rect.bottom - rect.top;
}


bool transform_position(float* a1, float* a2, __int64 a3)
{
	float v3; // xmm3_4
	float v4; // xmm2_4
	float v5; // xmm4_4
	float v6; // xmm0_4
	bool result; // al

	v3 = (float)((float)((float)(a1[1] * a2[1]) + (float)(*a1 * *a2)) + (float)(a1[2] * a2[2])) + a1[3];
	*(float *)a3 = v3;
	v4 = (float)((float)((float)(a1[5] * a2[1]) + (float)(a1[4] * *a2)) + (float)(a1[6] * a2[2])) + a1[7];
	*(float *)(a3 + 4) = v4;
	v5 = (float)((float)((float)(a1[13] * a2[1]) + (float)(a1[12] * *a2)) + (float)(a1[14] * a2[2])) + a1[15];
	*(int*)(a3 + 8) = 0;
	if (v5 >= 0.001)
		v6 = 1.0 / v5;
	else
		v6 = 100000.0;
	result = v5 < 0.001;
	*(float *)(a3 + 4) = v4 * v6;
	*(float *)a3 = v3 * v6;
	return result;
}

bool world_to_screen(const D3DXMATRIX& viewmatrix, const Vec3& world, Vec3& screen)
{
	if (!transform_position((float*)&viewmatrix, (float*)&world, (__int64)&screen))
	{
		screen.x = float(1920) / 2.f + (0.5f * screen.x * 1920 + 0.5f);
		screen.y = float(1080) / 2.f - (0.5f * screen.y * 1080 + 0.5f);

		return true;
	}

	return false;
}

Vec3 ClampAngle(Vec3 qaAng)
{
	Vec3 ret;
	ret.x = qaAng.x;
	ret.y = qaAng.y;

	if (qaAng.x > 89.0f && qaAng.x <= 180.0f)
		ret.x = 89.0f;

	if (qaAng.x > 180.0f)
		ret.x = qaAng.x - 360.0f;

	if (qaAng.x < -89.0f)
		ret.x = -89.0f;

	if (qaAng.y > 180.0f)
		ret.y = qaAng.y - 360.0f;

	if (qaAng.y < -180.0f)
		ret.y = qaAng.y + 360.0f;

	ret.z = 0;

	return ret;
}

void VectorAngles(const Vec3& forward, Vec3& angles)
{
	float tmp, yaw, pitch;

	if (forward.y == 0 && forward.x == 0)
	{
		yaw = 0;
		if (forward.x > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward.y, forward.x) * 180 / 3.14159265358979323846264338327950288);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward.x * forward.x + forward.y * forward.y);
		pitch = (atan2(-forward.z, tmp) * 180 / 3.14159265358979323846264338327950288);
		if (pitch < 0)
			pitch += 360;
	}
	angles.x = pitch;
	angles.y = yaw;
	angles.z = 0;
}