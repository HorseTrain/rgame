#pragma once

#ifndef CHUNK_H
#define CHUNK_H

#define CHUNK_SIZE		16

#define BLOCKS_PER_CHUNK (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)

#include "block.h"
#include "rgame/glm/vec3.hpp"
#include <vector>

#include "rgame/render_mesh.h"

struct chunk_manager;

struct chunk
{
	render_mesh		chunk_mesh;
	chunk_manager*	manager;

	bool			data_generated;
	bool			mesh_generated;

	int				x, y, z;

	bool			is_secure;

	static glm::vec3		get_position(chunk* chunk_context);
	static float			get_distance_from_player(chunk* chunk_context);
	static bool				in_render_distance(chunk* chunk_context);

	block*					block_data;
	std::vector<glm::ivec3>	blocks_to_generate;

	chunk*					neighbors[6];

	static void		create(chunk* result, int x, int y, int z, chunk_manager* manager);
	static void		destroy(chunk* to_destroy);
	static void		generate_chunk_data(chunk* chunk_context);
	static void		generate_chunk_mesh(chunk* chunk_context);
	static int		get_block_index(int x, int y, int z);
	static block*	get_block_reference(chunk* chunk_context, int x, int y, int z);
	static void		create_block(chunk* chunk_context, int cx, int cy, int cz);
	static bool		valid_for_mesh_generation(chunk* chunk_context);

	static chunk*	allocate_chunk();
	static void		destroy_allocation(chunk* to_destroy);
};

#endif