#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "rgame/rgame.h"

#include "level.h"
#include "rgame/lifetime_memory_manager.h"

struct level;
struct game;

struct game_object
{
	game*					game_context;
	level*					level_context;
	void*					game_object_data;
	std::string				name;

	bool					marked_for_destruction;
	bool					started;

	void*					object_data_start;
	void*					object_data_update;

	lifetime_memory_manager	memory;

	template	<typename T>
	static T*	allocate_game_object_data(game_object* game_object_context)
	{
		if (game_object_context->game_object_data != nullptr)
		{
			throw 0;

			assert(false);
		}

		T* result = lifetime_memory_manager::allocate<T>(&game_object_context->level_context->memory);

		game_object_context->game_object_data = result;

		result->game_object_context = game_object_context;

		game_object_context->object_data_start = T::start;
		game_object_context->object_data_update = T::update;

		return result;
	}

	static void create(game_object** result, std::string name, level* level_context);

	static void start(game_object* game_object_context);
	static void update(game_object* game_object_context);
	static void destroy(game_object* game_object_context);
};

#endif