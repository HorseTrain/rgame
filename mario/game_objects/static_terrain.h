#pragma once

#ifndef STATIC_TERRAIN_MESH_H
#define STATIC_TERRAIN_MESH_H

#include "rgame/mesh_renderer.h"
#include "rgame/transform.h"

struct static_render_mesh;
struct level;
struct game_object;

struct static_terrain
{
	game_object*	game_object_context;

	transform		object_transform;

	mesh_renderer	terrain_renderer;

	static void		create(static_terrain** static_terrain_mesh_context, level* level_context);

	static void		start(static_terrain* static_terrain_mesh_context);
	static void		update(static_terrain* static_terrain_mesh_context);
	static void		destroy(static_terrain* static_terrain_mesh_context);
};

#endif