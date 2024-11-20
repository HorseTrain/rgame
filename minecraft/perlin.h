#pragma once

#ifndef PERLIN_H
#define PERLIN_H

struct perlin
{
	int		seed;

	int		min;
	int		max;

	static void		create(perlin* result, int seed, int min, int max);
	static int		get_i_axis(perlin* perlin_context, int seed);
	static float	get_f_axis(perlin* perlin_context, float source);
	static float	get_x_y_perlin(perlin* perlin_context, float x, float y);
	static float	get_x_y_z_perlin(perlin* perlin_context, float x, float y, float z);
};

#endif