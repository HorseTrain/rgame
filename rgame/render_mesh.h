#pragma once

#ifndef RENDER_MESH
#define RENDER_MESH

#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

struct render_vertex
{
	glm::vec3 position;
	glm::vec3 normal;

	glm::vec2 uv;

	glm::vec4 color;
};

struct render_mesh
{
	uint32_t					vertex_array_buffer;
	uint32_t					vertex_buffer_object;
	uint32_t					index_array_buffer;

	bool						uploaded;
	uint32_t					primitive_type;

	std::vector<render_vertex>	vertices;
	std::vector<uint16_t>		indices;

	static void					create(render_mesh* result, uint32_t primitive_type);
	static void					upload(render_mesh* mesh);
	static void					destroy(render_mesh* mesh);
	static void					bind(render_mesh* mesh);
	static void					draw(render_mesh* mesh);
	static void					mark_for_reupload(render_mesh* mesh);
	static void					calculate_normals(render_mesh* mesh);
};

#endif