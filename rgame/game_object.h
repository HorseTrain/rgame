#pragma once

#ifndef SIMPLE_RENDER_OBJECT_H
#define SIMPLE_RENDER_OBJECT_H

#include "transform.h"
#include "render_mesh.h"

struct game_object;
struct game_scene;

typedef void(*object_function)(game_object*, game_scene*);

struct game_object
{
	bool			started;
	char			object_buffer[1024];

	object_function object_start;
	object_function object_update;
	object_function object_draw;

	static void		create_empty(game_object* result);
};

#endif
