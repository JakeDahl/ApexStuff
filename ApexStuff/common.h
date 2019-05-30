#include "stdafx.h"
#include "Overlay.h"
#include "D3D9Overlay.h"
#include "Color.h"
#include <d3dx9.h>
#include <DriverController.h>
#pragma comment(lib, "DriverController.lib")

using namespace DriverControl;

extern DriverController driver;
extern LPD3DXFONT pFont;


class Helpers
{
	public:
		uintptr_t GetEntityById(int id);
		float GetBulletSpeed();
		uintptr_t GetLocalEntity();

		D3DXMATRIX GetViewMatrix();

		uintptr_t modbase;
		uintptr_t gView = 0xC1C0670; //0xC1BABF0;
		uintptr_t gEntityList = 0x1FA0148; //0x1F9AE68;
		uintptr_t gGameLocal = 0x2353448; //0x234DEE8;
		uintptr_t gViewMatrix = 0xC1C0670; //0xC1BABF0;
		uintptr_t Entity_ID = 0x8;
		uintptr_t Entity_Team = 0x3E4;
		uintptr_t Entity_Velocity = 0x120; //Vec3 x y z, z = UP
		uintptr_t Position_Feet = 0x12C;
		uintptr_t Position_Pro = 0x408;
		DWORD Position_Head = 0x3AF0;
		DWORD Entity_Health = 0x3D4;
		DWORD Entity_Sheild = 0x150;
		DWORD Entity_MaxHealth = 0x4FC;
		DWORD Entity_Feetpos = 0x12C;
		DWORD Entity_Headpos = 0x3AF0;
		DWORD ViewAngles = 0x1E1C;
		DWORD Recoil = 0x2014;
		DWORD Rotation = 0x20B8;
		
	private:

};



class Vec3
{
public:
	float x, y, z;
	Vec3() { x = y = z = 0.0f; };
	Vec3(float X, float Y, float Z) { x = X; y = Y; z = Z; };
	void Zero() { x = y = z = 0.0f; };
	void NineNull() { x = y = z = 999.9f; };
	friend Vec3 Vec3::operator - (Vec3 a, Vec3 b)
	{
		Vec3 temp;
		temp.x = a.x - b.x;
		temp.y = a.y - b.y;
		temp.z = a.z - b.z;
		return temp;
	};
	friend Vec3 Vec3::operator + (Vec3 a, Vec3 b)
	{
		Vec3 temp;
		temp.x = a.x + b.x;
		temp.y = a.y + b.y;
		temp.z = a.z + b.z;
		return temp;
	};
	friend void Vec3::operator += (Vec3& a, Vec3 b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
	};
	friend void Vec3::operator -= (Vec3& a, Vec3 b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
	};
	friend Vec3 Vec3::operator * (Vec3 a, float b)
	{
		Vec3 temp;
		temp.x = a.x * b;
		temp.y = a.y * b;
		temp.z = a.z * b;
		return temp;
	};

};

struct Vec2
{
	float x, y;
};

bool world_to_screen(const D3DXMATRIX& viewmatrix, const Vec3& world, Vec3& screen);
//int Distance(Vec3 first, Vec3 second);
Vec3 ClampAngle(Vec3 qaAng);
Vec3 CalcAngle(Vec3 src, Vec3 dst);
void VectorAngles(const Vec3& forward, Vec3& angles);

float Distance(Vec3 EntityPos, Vec3 PlayerPos);