#pragma once

#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <unordered_map>

#include "maze_data.h"

#include "rgame/pixel.h"
#include "rgame/glm/vec2.hpp"

#define CELL_SIZE 10

struct static_render_mesh;
struct maze_level;
struct render_texture;
struct game_object;
struct render_surface;
struct maze_solver;

struct maze
{
	maze_level*								maze_level_context;
											
	game_object*							game_object_context;
	render_surface*							debug_texture_surface;
											
	maze_cell*								cells;
	std::unordered_map<wall_key,maze_wall*>	walls;
	std::vector<maze_cell*>					solution;

	static_render_mesh*						maze_mesh;
	maze_solver*							maze_solver_context;

	int										cell_width;
	int										cell_height;

	static void								create(maze* result, maze_level* maze_level_context);
	
	static void								destroy_current_maze(maze* maze_context);
	static void								generate_maze(maze* maze_context, int width, int height);

	static void								generate_maze_texture(render_texture** result, maze* maze_context);
	static void								generate_maze_mesh(maze* maze_context);

	static void								render_debug_texture(maze* maze_context);

	static maze_cell*						get_cell(maze* maze_context, int x, int y);

	static void								start(maze* game_object_context);
	static void								update(maze* game_object_context);
	static void								destroy(maze* maze_context);
};

#endif