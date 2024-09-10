#pragma once

#ifndef BASIC_RENDER_MESH_H
#define BASIC_RENDER_MESH_H

#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

struct basic_render_vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct basic_render_mesh
{
	uint32_t					vertex_array_buffer;
	uint32_t					vertex_buffer_object;
	uint32_t					index_array_buffer;

	bool						uploaded;
	uint32_t					primitive_type;

	std::vector<basic_render_vertex>	vertices;
	std::vector<uint16_t>		indices;

	static void					create(basic_render_mesh* result, uint32_t primitive_type);
	static void					upload(basic_render_mesh* mesh);
	static void					destroy(basic_render_mesh* mesh);
	static void					bind(basic_render_mesh* mesh);
	static void					draw(basic_render_mesh* mesh);
	static void					mark_for_reupload(basic_render_mesh* mesh);
	static void					calculate_normals(basic_render_mesh* mesh);
};

#endif