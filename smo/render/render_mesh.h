#pragma once

#ifndef RENDER_MESH
#define RENDER_MESH

#include <inttypes.h>

#include "tools/memory_tools.h"
#include "render_vertex.h"

struct render_mesh
{
	array_container<render_vertex>	vertices;
	array_container<uint16_t>		indices;

	int								primitive_type;

	static void						create(render_mesh* result, int primitive_type, int vertex_count, int index_count);
	static void						destroy(render_mesh* render_mesh_context);

	static void						upload(render_mesh* render_mesh_context);
	static void						unload(render_mesh* render_mesh_context);
	static void						draw(render_mesh* render_mesh_context);
};

#endif