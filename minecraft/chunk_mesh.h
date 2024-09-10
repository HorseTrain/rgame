#pragma once

#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include "rgame/glm/vec3.hpp"
#include <vector>

struct chunk_vertex
{
	int		position_normal; 
	//bits 0 - 15	position data, 
	//bits 15 - 18	normal
	//bits 18 - 32	texture data
};

struct chunk_mesh
{
	std::vector< chunk_vertex>	vertex_data;
	int							vbo;

	static void					create(chunk_mesh* chunk_mesh_context);
	static void					destroy(chunk_mesh* chunk_mesh_context);
	static void					upload(chunk_mesh* chunk_mesh_context);
	static void					unload(chunk_mesh* chunk_mesh_context);
};

#endif