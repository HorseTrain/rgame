#pragma once

#ifndef R_MATH_H
#define R_MATH_H

template <typename T>
T degrees_to_radians(T source)
{
	return source / ((T)180 / (T)3.14159265359);
}

#endif