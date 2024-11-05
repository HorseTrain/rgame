#pragma once

#ifndef PIXEL_H
#define PIXEL_H

#include <inttypes.h>

#include <stddef.h>
#include <stdio.h>

#pragma pack(2)
template <typename T, int C>
struct pixel
{
	T data[C];

	static pixel* get_pixel(pixel* buffer, int x, int y, int width)
	{
		return buffer + ((y * width) + x);
	}
};

typedef pixel<uint8_t, 4> rgba_i8;
typedef pixel<uint8_t, 3> rgb_i8;

#endif