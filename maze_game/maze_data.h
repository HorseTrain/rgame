#pragma once

#ifndef MAZE_DATA_H
#define MAZE_DATA_H

#define DIRECTION_NORTH 0
#define DIRECTION_EAST	1
#define DIRECTION_SOUTH 2
#define DIRECTION_WEST	3

struct maze;
struct maze_cell;

struct wall_key
{
	int base_x;
	int base_y;

	int direction;
};

struct maze_wall
{
	wall_key	key;

	maze_cell*	p_cell;
	maze_cell*	n_cell;
};

struct maze_cell
{
	maze*		maze_context;
	maze_wall*	walls[4];
};

#endif