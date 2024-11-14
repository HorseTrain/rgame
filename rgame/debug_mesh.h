#pragma once

#ifndef DEBUG_MESH_H
#define DEBUG_MESH_H

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <inttypes.h>
#include <vector>

struct debug_mesh_vertex
{
	glm::vec3 position;
	glm::vec2 uv;
};

struct debug_mesh
{
	uint32_t						primitive_mode;

	std::vector<debug_mesh_vertex>*	vertices;
	std::vector<uint16_t>*			indices;

	uint32_t						gl_handles[3];

	static void						create(debug_mesh* result, int primitive_mode);
	static void						destroy(debug_mesh* debug_mesh_context);

	static void						upload(debug_mesh* to_upload);
	static void						unload(debug_mesh* to_unload);
	static void						bind(debug_mesh* to_bind);
	static void						generic_draw(debug_mesh* to_draw);

	static void						create_debug_quad(debug_mesh** result);
};

#endif