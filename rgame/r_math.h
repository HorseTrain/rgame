#pragma once

#ifndef R_MATH_H
#define R_MATH_H

#include "glm/detail/qualifier.hpp"
#include "math.h"

#define M_PI 3.14159265359
#define TWO_M_PI (2 * M_PI)

template <typename T>
T degrees_to_radians(T source)
{
	return source / ((T)180 / (T)3.14159265359);
}

typedef glm::vec<3, uint8_t> ivec3b;

static void mod_angle(float* working)
{
	while (*working > TWO_M_PI)
	{
		*working -= TWO_M_PI;
	}

	while (*working < 0)
	{
		*working += TWO_M_PI;
	}
}

static void clamp_angle(float* working, float min, float max)
{
	assert(max > min);

	if (*working < min)
	{
		*working = min;
	}

	if (*working > max)
	{
		*working = max;
	}
}

static float lerp(float min, float max, float percent)
{
	return min + ((max - min) * percent);
}

static float slerp(float min, float max, float percent)
{
	if (percent > 1)
	{
		percent = 1;
	}
	else if (percent < 0)
	{
		percent = 0;
	}
	else
	{
		percent = sin(M_PI * percent - M_PI / 2) / 2 + .5;
	}

	return lerp(min, max, percent);
}

static void lerp(float* result,float* min, float* max, float percent, int count)
{
	for (int i = 0; i < count; ++i)
	{
		result[i] = lerp(min[i], max[i], percent);
	}
}

static uint32_t hash(uint32_t a)
{
	a = (a + 0x7ed55d16) + (a << 12);
	a = (a ^ 0xc761c23c) ^ (a >> 19);
	a = (a + 0x165667b1) + (a << 5);
	a = (a + 0xd3a2646c) ^ (a << 9);
	a = (a + 0xfd7046c5) + (a << 3);
	a = (a ^ 0xb55a4f09) ^ (a >> 16);
	return a;
}

static void get_int_decimal(int* int_result, float* decimal_result, float source)
{
	*int_result = source;
	*decimal_result = source - (int)source;
}

#endif