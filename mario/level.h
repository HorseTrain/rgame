#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#include <unordered_set>
#include <mutex>
#include "rgame/lifetime_memory_manager.h"
#include "rgame/render_camera.h"

struct game;
struct game_object;

struct level
{
	lifetime_memory_manager				memory;

	game*								game_context;

	std::mutex							game_object_accessor_lock;
	std::unordered_set<game_object*>	active_game_objects;
	std::unordered_set<game_object*>	game_object_creation_que;
	std::unordered_set<game_object*>	game_object_destruction_que;

	render_camera						working_camera;


	bool								level_is_closing;

	static void							create(level* result, game* game_context);
	static void							append_to_que_with_lock(level* level_context, std::unordered_set<game_object*>* que, game_object* object);

	static void							update(level* level_context);
	static void							destroy(level* level_context);
};

#endif