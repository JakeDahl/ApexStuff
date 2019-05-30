#ifndef UTILS_H
#define UTILS_H

#pragma once

#include "common.h"

class Utils
{
public:
	static int Distance(Vec3 first, Vec3 second);
	static Vec3 Cross(Vec3 a, Vec3 b);
	static float Dot(Vec3 left, Vec3 right);
	static Vec3 WorldToScreen(Vec3 pos, TransData transData);
	static void GetTextSize(const char* str, LPSIZE size);
};

#endif