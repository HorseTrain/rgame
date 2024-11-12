#pragma once

#ifndef R_MATH_H
#define R_MATH_H

#include "glm/detail/qualifier.hpp"

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

static void lerp(float* result,float* min, float* max, float percent, int count)
{
	for (int i = 0; i < count; ++i)
	{
		result[i] = lerp(min[i], max[i], percent);
	}
}

#endif