#pragma once

#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include <inttypes.h>
#include <vector>

#include "rgame/glm/vec3.hpp"
#include "rgame/r_math.h"

struct chunk_vertex
{
	uint32_t			vertex_data_0; 
	//bits 0 - 15		position data, 
	//bits 15 - 18		normal
	//bits 18 - 32		texture data

	static chunk_vertex create(uint8_t x, uint8_t y, uint8_t z, uint8_t normal_index, uint8_t texture_x, uint8_t texture_y);
	static void			set_vertex_axis(chunk_vertex* chunk_vertex_context, int index, uint8_t data);
	static uint8_t		get_vertex_axis(chunk_vertex* chunk_vertex_context, int index);
	static void			add_offset(chunk_vertex* chunk_vertex_context, ivec3b offset);
};

struct chunk_mesh
{
	std::vector<chunk_vertex>	vertex_data;
	uint32_t					vao;
	uint32_t					vbo;

	bool						uploaded;

	static void					create(chunk_mesh* chunk_mesh_context);
	static void					destroy(chunk_mesh* chunk_mesh_context);
	static void					upload(chunk_mesh* chunk_mesh_context);
	static void					unload(chunk_mesh* chunk_mesh_context);
	static void					draw(chunk_mesh* chunk_mesh_context);
};

#endif