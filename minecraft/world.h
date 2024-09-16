#pragma once

#ifndef WORLD_H
#define WORLD_H

#include <inttypes.h>

#include "rgame/render_camera.h"

#include "chunk_manager.h"
#include "player.h"

struct game;

struct world
{
	game*			game_context;
	chunk_manager	chunk_manager_context;
	uint64_t		seed;
	player			main_player;
	render_camera	main_camera;

	static void create(world* result, game* game_context, uint64_t seed);
	static void destroy(world* world_context);
	static void update(world* world_context);
};

#endif