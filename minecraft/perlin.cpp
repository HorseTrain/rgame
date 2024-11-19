#include "perlin.h"
#include "xhash"
#include "rgame/r_math.h"

void perlin::create(perlin* result, int seed, int min, int max)
{
	result->seed = seed;
	
	result->min = min;
	result->max = max;
}

int perlin::get_i_axis(perlin* perlin_context, int seed)
{
	int min = perlin_context->min;
	int max = perlin_context->max;

	int range = max - min;

	return min + (hash(seed) % range);
}

float perlin::get_f_axis(perlin* perlin_context, float source)
{
	source = abs(source);

	int i;
	float d;

	get_int_decimal(&i, &d, source);

	int is = perlin::get_i_axis(perlin_context, i);
	int in = perlin::get_i_axis(perlin_context, i + 1);

	return slerp(is, in, d);
}

float perlin::get_x_y_perlin(perlin* perlin_context, float x, float y)
{
	return get_f_axis(perlin_context, x) + get_f_axis(perlin_context, 10000 + y);
}
