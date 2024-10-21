#pragma once

#ifndef DEBUG_RENDERER_H
#define DEBUG_RENDERER_H

#include "rgame/mesh_renderer.h"
#include "rgame/transform.h"

struct game_object;
struct level;
struct static_render_mesh;
struct render_shader;
struct render_surface;
struct camera;

struct debug_renderer
{
	game_object*		game_object_context;

	camera*				camera_context;

	transform			object_transform;

	mesh_renderer		mesh_renderer_context;

	static void			create(debug_renderer** result, level* level_context, camera* camera_context);

	static void			start(debug_renderer* debug_renderer_context);
	static void			update(debug_renderer* debug_renderer_context);
	static void			destroy(debug_renderer* debug_renderer_context);
};

#endif