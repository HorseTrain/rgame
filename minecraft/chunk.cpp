#include "chunk.h"
#include "block.h"
#include "chunk_mesh.h"
#include "chunk_store.h"
#include "chunk_manager.h"
#include "iostream"
#include "player.h"
#include "world.h"

#include "rgame/glm/glm.hpp"

#define NEIGHBOR_PX 0
#define NEIGHBOR_NX 1
#define NEIGHBOR_PY 2
#define NEIGHBOR_NY 3
#define NEIGHBOR_PZ 4
#define NEIGHBOR_NZ 5

static void create_block_data(chunk* chunk_context, block* block_reference, int x, int y, int z)
{
	block_reference->chunk_context = chunk_context;

	block_reference->local_position = ivec3b(x, y, z);

	glm::ivec3 world_position = block::get_global_position(block_reference);

	x = world_position.x;
	y = world_position.y;
	z = world_position.z;

	float amp = 5;
	float freq = 0.05;

	float height = (sin(x * freq) + sin(y * freq) + sin(z * freq)) * amp;

	if (y > height)
	{
		block_reference->is_transparent = true;
	}
	else
	{
		block_reference->is_transparent = false;
	}
}

const char face_insert_pattern[] =
{
	0b00,
	0b10,
	0b11,
	0b01
};

static void insert_face(chunk_mesh* working_mesh, int axis_0, int axis_1, int normal_index, ivec3b offset, int block_id, int block_face, int single_texture_size = 16)
{
	for (int i = 0; i < 4; ++i)
	{
		int active_faces = face_insert_pattern[i];

		uint8_t data_a0 = (active_faces & 1);
		uint8_t data_a1 = (active_faces >> 1) & 1;

		chunk_vertex working_vertex = chunk_vertex::create(0, 0, 0, normal_index, (block_face * single_texture_size) + (data_a0 * single_texture_size), (block_id * single_texture_size) + (data_a1 * single_texture_size));

		chunk_vertex::set_vertex_axis(&working_vertex, axis_0, data_a0);
		chunk_vertex::set_vertex_axis(&working_vertex, axis_1, data_a1);

		chunk_vertex::add_offset(&working_vertex, offset);

		working_mesh->vertex_data.push_back(working_vertex);
	}
}

static void create_mesh_data_at(chunk_mesh* chunk_mesh_context,chunk* chunk_context, block* block_reference)
{
	ivec3b block_position = block_reference->local_position;

	if (chunk::get_block_reference(chunk_context, block_position.x + 1, block_position.y, block_position.z)->is_transparent)
	{
		insert_face(chunk_mesh_context, 2, 1, 1, block_reference->local_position + ivec3b(1, 0, 0), block_reference->block_id, 1);
	}

	if (chunk::get_block_reference(chunk_context, block_position.x - 1, block_position.y, block_position.z)->is_transparent)
	{
		insert_face(chunk_mesh_context, 1, 2, 0, block_reference->local_position + ivec3b(0, 0, 0), block_reference->block_id, 3);
	}

	if (chunk::get_block_reference(chunk_context, block_position.x, block_position.y + 1, block_position.z)->is_transparent)
	{
		insert_face(chunk_mesh_context, 0, 2, 3, block_reference->local_position + ivec3b(0, 1, 0), block_reference->block_id, 0);
	}

	if (chunk::get_block_reference(chunk_context, block_position.x, block_position.y - 1, block_position.z)->is_transparent)
	{
		insert_face(chunk_mesh_context, 2, 0, 2, block_reference->local_position + ivec3b(0, 0, 0), block_reference->block_id, 5);
	}

	if (chunk::get_block_reference(chunk_context, block_position.x, block_position.y, block_position.z + 1)->is_transparent)
	{
		insert_face(chunk_mesh_context, 1, 0, 5, block_reference->local_position + ivec3b(0, 0, 1), block_reference->block_id, 2);
	}

	if (chunk::get_block_reference(chunk_context, block_position.x, block_position.y, block_position.z - 1)->is_transparent)
	{
		insert_face(chunk_mesh_context, 0, 1, 4, block_reference->local_position, block_reference->block_id, 4);
	}
}

bool chunk::neighbors_marked_for_destruction(chunk* chunk_context)
{
	for (int i = 0; i < 6; ++i)
	{
		if (chunk_context->neighbors[i] == nullptr)
			continue;

		if (chunk_context->neighbors[i]->marked_for_destruction)
		{
			return true;
		}
	}

	return false;
}

void chunk::create(chunk** result, chunk_manager* chunk_manager_context, glm::ivec3 position)
{
	chunk* working_result = new chunk();

	working_result->position = position;
	working_result->chunk_manager_context = chunk_manager_context;

	working_result->chunk_mesh_context = new chunk_mesh();
	chunk_mesh::create(working_result->chunk_mesh_context);

	working_result->block_data = new block[CUBE_CHUNK_SIZE * CUBE_CHUNK_SIZE * CUBE_CHUNK_SIZE];

	working_result->data_ready = false;
	working_result->mesh_ready = false;

	memset(working_result->neighbors, 0, sizeof(chunk::neighbors));

	working_result->marked_for_destruction = false;

	*result = working_result;
}

void chunk::destroy(chunk* chunk_mesh_context, bool deallocate)
{
	delete[] chunk_mesh_context->block_data;

	chunk_mesh::destroy(chunk_mesh_context->chunk_mesh_context);

	delete chunk_mesh_context->chunk_mesh_context;

	if (deallocate)
	{
		delete chunk_mesh_context;
	}
}

void chunk::generate_data(chunk* chunk_context)
{
	chunk_context->non_transparent = 0;

	for (int x = 0; x < CUBE_CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CUBE_CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CUBE_CHUNK_SIZE; ++z)
			{
				block* block = get_block_reference(chunk_context, x, y, z);

				create_block_data(chunk_context, block, x, y, z);

				if (!block->is_transparent)
					chunk_context->non_transparent++;
			}
		}
	}
}

void chunk::generate_mesh(chunk* chunk_context)
{
	if (!ready_for_mesh_generation(chunk_context))
	{
		assert(false); 
		
		throw 0;
	}

	if (chunk_context->non_transparent == 0)
		return;

	chunk_mesh* working_mesh = chunk_context->chunk_mesh_context;

	for (int x = 0; x < CUBE_CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CUBE_CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CUBE_CHUNK_SIZE; ++z)
			{
				block* block = get_block_reference(chunk_context, x, y, z);

				if (block->is_transparent)
					continue;

				create_mesh_data_at(working_mesh,chunk_context, block);
			}
		}
	}
}

glm::ivec3 chunk::request_chunk_neighbor_offset(chunk* chunk_context, int index)
{
	switch (index)
	{
	case NEIGHBOR_PX: return chunk_context->position + glm::ivec3(CUBE_CHUNK_SIZE, 0, 0);
	case NEIGHBOR_NX: return chunk_context->position + glm::ivec3(-CUBE_CHUNK_SIZE, 0, 0);
	case NEIGHBOR_PY: return chunk_context->position + glm::ivec3(0, CUBE_CHUNK_SIZE, 0);
	case NEIGHBOR_NY: return chunk_context->position + glm::ivec3(0, -CUBE_CHUNK_SIZE, 0);
	case NEIGHBOR_PZ: return chunk_context->position + glm::ivec3(0, 0, CUBE_CHUNK_SIZE);
	case NEIGHBOR_NZ: return chunk_context->position + glm::ivec3(0, 0, -CUBE_CHUNK_SIZE);
	}

	assert(false); throw 0;
}

bool chunk::request_chunk_neighbors(chunk* chunk_context)
{
	chunk_store* chunk_store_context = &chunk_context->chunk_manager_context->chunks;

	bool neighbors_all_exist_test = true;

	for (int i = 0; i < 6; ++i)
	{
		if (chunk_context->neighbors[i] == nullptr)
		{
			neighbors_all_exist_test = false;

			break;
		}
	}

	if (neighbors_all_exist_test)
	{
		return true;
	}

	for (int i = 0; i < 6; ++i)
	{
		glm::ivec3 neighbor_key = request_chunk_neighbor_offset(chunk_context, i);

		chunk* test_chunk = chunk_store::request_chunk(chunk_store_context, neighbor_key);

		if (test_chunk == nullptr)
		{
			return false;
		}

		if (!test_chunk->data_ready)
		{
			return false;
		}

		chunk_context->neighbors[i] = test_chunk;
	}

	return true;
}

bool chunk::ready_for_neighbor_generation(chunk* chunk_context)
{
	if (chunk_context->mesh_ready)
		return false;

	if (!chunk_context->data_ready)
		return false;

	return true;
}

bool chunk::ready_for_mesh_generation(chunk* chunk_context, std::unordered_set<chunk*>* chunks_just_destroyed)
{
	if (chunk_context->mesh_ready)
		return false;

	if (!chunk_context->data_ready)
		return false;

 	bool all_neighbors_exist = request_chunk_neighbors(chunk_context);

	if (!all_neighbors_exist)
	{
		return false;
	}

	for (int i = 0; i < 6; ++i)
	{
		if (!chunk_context->neighbors[i]->data_ready)
			return false;

		if (chunks_just_destroyed == nullptr)
			continue;

		if (chunks_just_destroyed->find(chunk_context->neighbors[i]) != chunks_just_destroyed->end())
		{
			std::cout << "nig" << std::endl;

			return false;
		}
	}

	return true;
}

int chunk::get_block_index(int x, int y, int z)
{
	return (x * CUBE_CHUNK_SIZE * CUBE_CHUNK_SIZE) + (y * CUBE_CHUNK_SIZE) + z;
}

/*
block* chunk::get_block_reference(chunk* chunk_context, int x, int y, int z)
{
	chunk_store* chunk_store_context = &chunk_context->chunk_manager_context->chunks;
	chunk* other_chunk = nullptr;

	bool out_of_bounds = false;

	if (x >= CUBE_CHUNK_SIZE)
	{
		out_of_bounds = true;

		x -= CUBE_CHUNK_SIZE;

		other_chunk = chunk_store::request_chunk(chunk_store_context, chunk::request_chunk_neighbor_offset(chunk_context, NEIGHBOR_PX));
	}

	if (x < 0)
	{
		out_of_bounds = true;

		x += CUBE_CHUNK_SIZE;

		other_chunk = chunk_store::request_chunk(chunk_store_context, chunk::request_chunk_neighbor_offset(chunk_context, NEIGHBOR_NX));
	}

	if (y >= CUBE_CHUNK_SIZE)
	{
		out_of_bounds = true;

		y -= CUBE_CHUNK_SIZE;

		other_chunk = chunk_store::request_chunk(chunk_store_context, chunk::request_chunk_neighbor_offset(chunk_context, NEIGHBOR_PY));
	}

	if (y < 0)
	{
		out_of_bounds = true;

		y += CUBE_CHUNK_SIZE;

		other_chunk = chunk_store::request_chunk(chunk_store_context, chunk::request_chunk_neighbor_offset(chunk_context, NEIGHBOR_NY));
	}

	if (z >= CUBE_CHUNK_SIZE)
	{
		out_of_bounds = true;

		z -= CUBE_CHUNK_SIZE;

		other_chunk = chunk_store::request_chunk(chunk_store_context, chunk::request_chunk_neighbor_offset(chunk_context, NEIGHBOR_PZ));
	}

	if (z < 0)
	{
		out_of_bounds = true;

		z += CUBE_CHUNK_SIZE;

		other_chunk = chunk_store::request_chunk(chunk_store_context, chunk::request_chunk_neighbor_offset(chunk_context, NEIGHBOR_NZ));
	}

	if (out_of_bounds)
	{
		chunk_context->block_data[0];

		if (other_chunk == nullptr)
		{
			assert(false);

			std::cout << "Error lol" << std::endl;

			throw 0;
		}

		return get_block_reference(other_chunk, x, y, z);
	}

	int chunk_index = get_block_index(x, y, z);

	block* result = &chunk_context->block_data[chunk_index];

	return result;
}
*/

block* chunk::get_block_reference(chunk* chunk_context, int x, int y, int z)
{
	chunk_store* chunk_store_context = &chunk_context->chunk_manager_context->chunks;
	chunk* other_chunk = nullptr;

	bool out_of_bounds = false;

	if (x >= CUBE_CHUNK_SIZE)
	{
		out_of_bounds = true;

		x -= CUBE_CHUNK_SIZE;

		other_chunk = chunk_context->neighbors[NEIGHBOR_PX];
	}

	if (x < 0)
	{
		out_of_bounds = true;

		x += CUBE_CHUNK_SIZE;

		other_chunk = chunk_context->neighbors[NEIGHBOR_NX];
	}

	if (y >= CUBE_CHUNK_SIZE)
	{
		out_of_bounds = true;

		y -= CUBE_CHUNK_SIZE;

		other_chunk = chunk_context->neighbors[NEIGHBOR_PY];
	}

	if (y < 0)
	{
		out_of_bounds = true;

		y += CUBE_CHUNK_SIZE;

		other_chunk = chunk_context->neighbors[NEIGHBOR_NY];
	}

	if (z >= CUBE_CHUNK_SIZE)
	{
		out_of_bounds = true;

		z -= CUBE_CHUNK_SIZE;

		other_chunk = chunk_context->neighbors[NEIGHBOR_PZ];
	}

	if (z < 0)
	{
		out_of_bounds = true;

		z += CUBE_CHUNK_SIZE;

		other_chunk = chunk_context->neighbors[NEIGHBOR_NZ];
	}

	if (out_of_bounds)
	{
		chunk_context->block_data[0];

		if (other_chunk == nullptr)
		{
			assert(false); 

			std::cout << "Error lol" << std::endl;
			
			throw 0;
		}

		return get_block_reference(other_chunk, x, y, z);
	}

	int chunk_index = get_block_index(x, y, z);

	block* result = &chunk_context->block_data[chunk_index];

	return result;
}

bool chunk::in_render_distance(chunk* chunk_context)
{
	player* working_player = &chunk_context->chunk_manager_context->world_context->main_player;

	glm::vec3 player_position = working_player->transform_context.position;
	glm::vec3 my_position = glm::vec3(chunk_context->position);

	float render_distance = chunk_context->chunk_manager_context->render_distance * CUBE_CHUNK_SIZE;

	return glm::distance(player_position, my_position) < render_distance;
}

void chunk::ghost_neighbors(chunk* chunk_context)
{
	for (int n = 0; n < 6; ++n)
	{
		chunk* working_neighbor = chunk_context->neighbors[n];

		if (working_neighbor == nullptr)
			continue;

		for (int i = 0; i < 6; ++i)
		{
			if (working_neighbor->neighbors[i] == chunk_context)
			{
				working_neighbor->neighbors[i] = nullptr;

				continue;
			}
		}
	}
}
