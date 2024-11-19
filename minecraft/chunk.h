#pragma once

#ifndef CHUNK_H
#define CHUNK_H

#define CUBE_CHUNK_SIZE 32

#include "rgame/glm/vec3.hpp"
#include "rgame/r_math.h"
#include <atomic>
#include <unordered_set>
#include <vector>

struct block;
struct chunk_mesh;
struct chunk_manager;

#define HOLE_SIZE 3
#define TIME_COUNT 3

static bool in_range(int source)
{
	return source > -HOLE_SIZE && source < HOLE_SIZE;
}

struct chunk
{
	chunk_mesh*			chunk_mesh_context;

	chunk_manager*		chunk_manager_context;
	block*				block_data;

	glm::ivec3			position;

	bool				data_ready;
	bool				mesh_ready;

	bool				generation_lock;

	chunk*				neighbors[6];

	uint64_t			destruction_times[TIME_COUNT];
	std::vector<ivec3b>	block_locations;

	std::atomic_bool	marked_for_destruction;
	std::atomic_bool	generating_data;

	int					non_transparent;

	float				highlight;

	static uint64_t		get_minimum_destruction_time(chunk* chunk_context, uint64_t* times);
	static bool			neighbors_marked_for_destruction(chunk* chunk_store_context);
	static void			create(chunk** result, chunk_manager* chunk_manager_context, glm::ivec3 position);
	static void			destroy(chunk* chunk_context, bool deallocate = true);
	static void			generate_data(chunk* chunk_context);
	static void			regenerate_mesh(chunk* chunk_context, bool recurse_neighbors);
	static void			generate_mesh(chunk* chunk_context);
	static glm::ivec3	request_chunk_neighbor_offset(chunk* chunk_context, int index);
	static bool			request_chunk_neighbors(chunk* chunk_context);
	static bool			ready_for_neighbor_generation(chunk* chunk_context);
	static bool			ready_for_mesh_generation(chunk* chunk_context, std::unordered_set<chunk*>* chunks_just_destroyed = nullptr);
	static int			get_block_index(int x, int y, int z);
	static block*		get_block_reference(chunk* chunk_context, int x, int y, int z, bool allow_nullptr = false);
	static block*		get_block_reference(chunk* chunk_context, glm::ivec3 position);
	static bool			in_render_distance(chunk* chunk_context);
	static void			ghost_neighbors(chunk* chunk_context);
	static bool			ready_for_deallocation(chunk* chunk_context);
};

#endif