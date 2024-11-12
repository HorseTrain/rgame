#pragma once

#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include "rgame/transform.h"

struct game_object;
struct maze;
struct static_render_mesh;

struct maze_solver
{
	game_object*		game_object_context;
	static_render_mesh* mesh;
	maze*				maze_context;

	transform			_transform;
	float				time;
	bool				in_reverse;

	static void create(maze_solver* result, maze* maze_context);

	static void start(maze_solver* maze_solver_context);
	static void update(maze_solver* maze_solver_context);
	static void destroy(maze_solver* maze_solver_context);
};

#endif