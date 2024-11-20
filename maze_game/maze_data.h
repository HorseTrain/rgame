#pragma once

#include <assert.h>

#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include "rgame/r_math.h"

#ifndef MAZE_DATA_H
#define MAZE_DATA_H

#define DIRECTION_NORTH 0
#define DIRECTION_EAST	1
#define DIRECTION_SOUTH 2
#define DIRECTION_WEST	3

#define EAST_WEST	0
#define NORTH_SOUTH 1

#define KEY_TYPE		wall_key
#define NEIGHBOR_COUNT	4

struct maze;
struct maze_cell;

struct wall_key
{
	int base_x;
	int base_y;

	int direction;

	bool operator == (const wall_key& other) const
	{
		return other.base_x == base_x && other.base_y == base_y && other.direction == direction;
	}
};

template <>
struct std::hash<KEY_TYPE>
{
	std::size_t operator()(const KEY_TYPE& k) const
	{
		int* buffer = (int*)&k;

		assert(sizeof(KEY_TYPE) % 4 == 0);

		uint64_t working_result = 0;

		for (int i = 0; i < sizeof(KEY_TYPE) / 4; ++i)
		{
			working_result ^= (uint64_t)hash_uint32(buffer[i]) | (uint64_t)hash_uint32(buffer[i] + 1) << 32;
		}

		return working_result;
	}
};

struct maze_wall
{
	bool		is_open;
};

struct maze_cell
{
	maze*		maze_context;
	maze_wall*	walls[4];

	int			x, y;

	bool		visited;

	maze_cell*	neighbors[NEIGHBOR_COUNT];

	static void find_neighbors(maze_cell* cell);
	static bool is_dead_end(maze_cell* cell);
};

#endif