#include "perlin.h"
#include "xhash"
#include "rgame/r_math.h"

void perlin::create(perlin* result, int seed)
{
	result->seed = seed;
}

int perlin::get_i_axis(perlin* perlin_context, int seed)
{
	return hash_uint32(seed) & UINT16_MAX;
}

float perlin::get_f_axis(perlin* perlin_context, float source)
{
	source = abs(source);

	int i;
	float d;

	get_int_decimal(&i, &d, source);

	int is = perlin::get_i_axis(perlin_context, i);
	int in = perlin::get_i_axis(perlin_context, i + 1);

	return slerp(is, in, d) / UINT16_MAX;
}

float perlin::get_x_y_perlin(perlin* perlin_context, float x, float y)
{
	return (get_f_axis(perlin_context, x) + get_f_axis(perlin_context, y)) / 2;
}

float perlin::get_x_y_z_perlin(perlin* perlin_context, float x, float y, float z)
{
	return get_f_axis(perlin_context, x) + get_f_axis(perlin_context, y) + get_f_axis(perlin_context,z);
}
