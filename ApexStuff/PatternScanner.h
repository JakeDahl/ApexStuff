#pragma once
#include <iostream>
#include <Windows.h>
#include <stdint.h>
#include <TlHelp32.h>
#include "common.h"

//class Memory;
class PatternScanner;
class Vec3;
class Vec2;

//
//class Memory
//{
//public:
//	HANDLE pHandle;
//	int procID;
//	int threadID;
//	template<typename T>
//	void rpm(T& variable, DWORD address)
//	{
//		ReadProcessMemory(pHandle, (LPVOID)address, &variable, sizeof(T), 0);
//	};
//	template<typename T>
//	void wpm(T& variable, DWORD address)
//	{
//		WriteProcessMemory(pHandle, (LPVOID)address, &variable, sizeof(T), 0);
//	};
//	bool GetHandle(const char* procName);
//	MODULEENTRY32 GetModule(const char* modName);
//	BOOL GetThreads(DWORD dwOwnerPID);
//};

// Example pattern: "\x45\x43\x45\x55\x33\x9a\xfa\xCC\xCC\xCC\xCC\x45\x110\x21"
// Example wildCard: 0xCC
//class PatternScanner
//{
//public:
//	Memory patMem;
//	PatternScanner(const char* process);
//	bool LoadModule(const char* module);
//	void UnloadModule();
//	DWORD FindPattern(char* pattern, bool deref, uint16_t offset = 0x00, uint8_t wildCard = 0xCC);
//	DWORD GetModBase();
//	~PatternScanner();
//private:
//
//	MODULEENTRY32 me32;
//	PBYTE data;
//	LPVOID Search(uint8_t* pPattern, size_t patternSize, uint8_t wildCard, uint8_t* pScanPos, size_t scanSize);
//};


class Vec2
{
public:
	float x, y;
	Vec2() { x = y = 0.0f; };
	void Zero() { x = y = 0.0f; };
	void NineNull() { x = y = 999.9f; };
	friend Vec2 Vec2::operator - (Vec2 a, Vec2 b)
	{
		Vec2 temp;
		temp.x = a.x - b.x;
		temp.y = a.y - b.y;
		return temp;
	};
	friend Vec2 Vec2::operator + (Vec2 a, Vec2 b)
	{
		Vec2 temp;
		temp.x = a.x + b.x;
		temp.y = a.y + b.y;
		return temp;
	};
	friend void Vec2::operator += (Vec2& a, Vec2 b)
	{
		a.x += b.x;
		a.y += b.y;
	};
	friend void Vec2::operator -= (Vec2& a, Vec2 b)
	{
		a.x -= b.x;
		a.y -= b.y;
	};
	friend Vec2 Vec2::operator / (Vec2 a, float b)
	{
		Vec2 temp;
		temp.x = a.x / b;
		temp.y = a.y / b;
		return temp;
	};
};

class Vec3
{
public:
	float x, y, z;
	Vec3() { x = y = z = 0.0f; };
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