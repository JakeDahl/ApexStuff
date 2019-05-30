#include "common.h"

uintptr_t Helpers::GetEntityById(int id)
{
	return driver.rpm<uintptr_t>(modbase + gEntityList + (id * 0x20));
}

float Helpers::GetBulletSpeed()
{
	uintptr_t LocalEntity = GetLocalEntity();
	
	int LastWeapon = driver.rpm<int>(LocalEntity + 0x163C) & 0xFFFF;
	
	float BulletSpeed = driver.rpm<float>(GetEntityById(LastWeapon) + 0x1BBC);
	return BulletSpeed;
}

uintptr_t Helpers::GetLocalEntity()
{
	return driver.rpm<uintptr_t>(modbase + gGameLocal);
}

D3DXMATRIX Helpers::GetViewMatrix()
{
	uintptr_t matrixptr = driver.rpm<uintptr_t>(modbase + gViewMatrix);
	uintptr_t matrixptr2 = driver.rpm<uintptr_t>(matrixptr + 0x35112 * 0x8);
	D3DXMATRIX matrix = driver.rpm<D3DXMATRIX>(matrixptr2);
	return matrix;
}
