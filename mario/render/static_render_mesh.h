#pragma once

#ifndef STATIC_RENDER_MESH_H
#define STATIC_RENDER_MESH_H

#include "rgame/memory_tools.h"
#include "inttypes.h"
#include "render_vertex.h"

struct static_render_mesh
{
	int							primative_type;

	fast_array<uint16_t>		indecies;
	fast_array<render_vertex>	vertices;

	uint32_t					gl_handles[3];

	static void					upload(static_render_mesh* static_render_mesh_context);
	static void					unload(static_render_mesh* static_render_mesh_context);
	static bool					is_uploaded(static_render_mesh* static_render_mesh_context);
	static void					bind(static_render_mesh* static_render_mesh_context);
	static void					create(static_render_mesh* result, int index_count, int vertex_count, int primative_type);
	static void					destroy(static_render_mesh* static_render_mesh_context);
	static void					create_debug_triangle(static_render_mesh* result);
	static void					create_debug_plane(static_render_mesh* result, float size);
	static void					create_debug_cube(static_render_mesh* result, float size);
	static void					generic_draw(static_render_mesh* result);
};

#endif