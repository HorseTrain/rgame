#pragma once

#ifndef RENDER_MESH
#define RENDER_MESH

#include <inttypes.h>

#include "tools/memory_tools.h"
#include "render_vertex.h"

struct static_render_mesh
{
	array_container<render_vertex>	vertices;
	array_container<uint16_t>		indices;

	int								primitive_type;

	int								gl_buffers[3];

	static void						create(static_render_mesh* result, int primitive_type, int vertex_count, int index_count);
	static void						destroy(static_render_mesh* render_mesh_context);

	static void						upload(static_render_mesh* render_mesh_context);
	static void						unload(static_render_mesh* render_mesh_context);
	static void						draw(static_render_mesh* render_mesh_context);
	static bool						is_uploaded(static_render_mesh* render_mesh_context);
	static void						bind(static_render_mesh* render_mesh_context);
	static void						calculate_normals(static_render_mesh* static_render_mesh_context);

	static void						create_triangle(static_render_mesh* result);
};

#endif