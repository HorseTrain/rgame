#pragma once

#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include "rgame/glm/vec2.hpp"

struct maze;

struct generation_bot
{

};

struct maze_generator
{
	maze* maze_context;

	static void create(maze_generator* result, maze* maze_context);
	static void generate_randomizd_depth_first_search(maze_generator* generator_context);
};

#endif