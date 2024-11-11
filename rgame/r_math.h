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

static void clamp_angle(float* working)
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


#endif