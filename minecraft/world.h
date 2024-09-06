#pragma once

#ifndef WORLD_H
#define WORLD_H

#include <inttypes.h>

#include "player.h"
#include "chunk_manager.h"
#include "rgame/input_manager.h"
#include "world_graphics_manager.h"

struct game_scene;
struct render_window;

struct world
{
	game_scene*				raw_scene;
	input_manager			input_manager_context;
	world_graphics_manager	world_graphics_manager_context;

	chunk_manager			chunk_manager_context;
	bool					world_started;

	player					main_player;

	static void create(world* result, game_scene* raw_scene);
	static void destroy(world* to_destroy);

	static void world_start(world* ctx);
	static void world_update(world* ctx);

	static void scene_world_loop(render_window* window, uint64_t frame_time, void* arguments);


};

#endif