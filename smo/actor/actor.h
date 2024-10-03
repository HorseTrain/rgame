#pragma once

#ifndef ACTOR_H
#define ACTOR_H

#include <inttypes.h>

struct level_scene;

struct actor;
struct game_context;

typedef void(*actor_function)(actor* actor_context);

struct actor
{
	game_context*				game_context_context;
	level_scene*				level_scene_context;
	void*						actor_data;

	int							actor_id;

	bool						started;

	actor_function				actor_create;
	actor_function				actor_update;
	actor_function				actor_destroy;
};

#endif