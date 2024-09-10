#pragma once

#ifndef BIG_INT
#define BIG_INT

static void big_int_add(uint8_t* result, uint8_t* left, uint8_t* right, int count)
{
	int carry = 0;

	for (int i = 0; i < count; ++i)
	{
		uint8_t n = left[i] + carry;
		uint8_t m = right[i];

		uint8_t working = n + m;

		carry = working < m;

		result[i] = working;
	}
}

static void big_int_not(uint8_t* result, uint8_t* source, int count)
{
	for (int i = 0; i < count; ++i)
	{
		result[i] = ~source[i];
	}
}

static void bit_int_incrament_by_one(uint8_t* result, uint8_t* source, int count)
{
	uint8_t one[100];
	memset(one, 0, count);

	one[0] = 1;

	big_int_add(result, result, one, count);
}

static void big_int_subtract(uint8_t* result, uint8_t* left, uint8_t* right, int count)
{
	big_int_not(result, right, count);
	bit_int_incrament_by_one(result, result, count);

	big_int_add(result, left, result, count);
}

#endif