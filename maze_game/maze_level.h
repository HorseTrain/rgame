#pragma once

#ifndef MAZE_LEVEL_H
#define MAZE_LEVEL_H

struct level;
struct maze;
struct debug_camera;

struct maze_level
{
	level*			level_context;

	debug_camera*	debug_camera_object;
	maze*			maze_context;

	static void		create(maze_level** maze_level_context,level* level_context);
	static void		destroy(maze_level* maze_level_context);
};

#endif