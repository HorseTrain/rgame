#pragma once

#ifndef MAZE_H
#define MAZE_H

struct static_render_mesh;
struct maze_level;

struct maze
{
	static_render_mesh* mesh_context;
	maze_level*			maze_level_context;

	static void			create(maze** maze_context, maze_level* maze_level_context);
	static void			destroy(maze* maze_context);

	static void			create_debug_triangle(maze* maze_context);
};

#endif