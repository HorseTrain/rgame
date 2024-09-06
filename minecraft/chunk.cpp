#include "chunk.h"
#include "rgame/render_window.h"
#include "chunk_manager.h"
#include "rgame/render_mesh.h"
#include "rgame/glm/glm.hpp"
#include <iostream>
#include <map>

void chunk::create(chunk* result, int x, int y, int z, chunk_manager* manager)
{
	render_mesh::create(&result->chunk_mesh, GL_QUADS);

	result->manager = manager;
	result->blocks_to_generate = std::vector<glm::ivec3>();

	result->data_generated = false;
	result->mesh_generated = false;

	result->x = x;
	result->y = y;
	result->z = z;

	result->block_data = nullptr;
}

void chunk::destroy(chunk* to_destroy)
{
	if (to_destroy->chunk_mesh.uploaded)
	{
		render_mesh::destroy(&to_destroy->chunk_mesh);
	}

	if (to_destroy->block_data != nullptr)
	{
		delete to_destroy->block_data;
	}
}

void chunk::generate_chunk_data(chunk* chunk_context)
{
	chunk_context->data_generated = true;
	chunk_context->block_data = new block[BLOCKS_PER_CHUNK];

	std::map<int, bool> hit;

	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				create_block(chunk_context, x, y, z);
			}
		}
	}
}

static void insert_face(render_mesh* mesh, glm::vec3 world_offset,int ia0, int ia1, glm::vec3 normal)
{
	render_vertex working_vertecies[4];

	int order[] = { 0b00, 0b10, 0b11, 0b01 };

	for (int i = 0; i < 4; ++i)
	{
		int wi = order[i];

		glm::vec3 v_offset = glm::vec3(0);

		int o0 = (wi & 1);
		int o1 = (wi >> 1) & 1;

		v_offset[ia0] = o0;
		v_offset[ia1] = o1;

		render_vertex to_insert = render_vertex();

		to_insert.position = world_offset + v_offset;
		to_insert.normal = normal;

		mesh->vertices.push_back(to_insert);
	}
}

void chunk::generate_chunk_mesh(chunk* chunk_context)
{
	if (!valid_for_mesh_generation(chunk_context))
	{
		return;
	}
	
	chunk_context->mesh_generated = true;

	render_mesh* working_mesh = &chunk_context->chunk_mesh;

	for (int i = 0; i < chunk_context->blocks_to_generate.size(); ++i)
	{
		glm::ivec3 working_position = chunk_context->blocks_to_generate[i];

		if (block::get_is_transparent(get_block_reference(chunk_context, working_position.x + 1, working_position.y, working_position.z)))
		{
			insert_face(working_mesh, glm::vec3(working_position) + glm::vec3(1, -1, 0), 2, 1, glm::vec3(1, 0, 0));
		}

		if (block::get_is_transparent(get_block_reference(chunk_context, working_position.x - 1, working_position.y, working_position.z)))
		{
			insert_face(working_mesh, glm::vec3(working_position) + glm::vec3(0, -1, 0), 1, 2, glm::vec3(-1, 0, 0));
		}

		if (block::get_is_transparent(get_block_reference(chunk_context, working_position.x, working_position.y + 1, working_position.z)))
		{
			insert_face(working_mesh, glm::vec3(working_position) + glm::vec3(0, 0, 0), 0, 2, glm::vec3(0, 1, 0));
		}

		if (block::get_is_transparent(get_block_reference(chunk_context, working_position.x, working_position.y - 1, working_position.z)))
		{
			insert_face(working_mesh, glm::vec3(working_position) + glm::vec3(0, -1, 0), 2, 0, glm::vec3(0, -1, 0));
		}

		if (block::get_is_transparent(get_block_reference(chunk_context, working_position.x, working_position.y, working_position.z + 1)))
		{
			insert_face(working_mesh, glm::vec3(working_position) + glm::vec3(0, -1, 1), 1, 0, glm::vec3(0, 0, 1));
		}

		if (block::get_is_transparent(get_block_reference(chunk_context, working_position.x, working_position.y, working_position.z - 1)))
		{
			insert_face(working_mesh, glm::vec3(working_position) + glm::vec3(0, -1, 0), 0, 1, glm::vec3(0, 0, -1));
		}
	}

	for (int i = 0; i < working_mesh->vertices.size(); ++i)
	{
		working_mesh->indices.push_back(i);
	}
}

int chunk::get_block_index(int x, int y, int z)
{
	return (x * CHUNK_SIZE * CHUNK_SIZE) + (y * CHUNK_SIZE) + z;
}

static bool test_if_out_of_bound(chunk* chunk_context, chunk** other_chunk,int& x, int& y, int& z)
{
	if (x < 0)
	{
		*other_chunk = chunk_manager::get_chunk(chunk_context->manager, chunk_context->x - CHUNK_SIZE, chunk_context->y, chunk_context->z);
		x += CHUNK_SIZE;

		return true;
	}

	if (x >= CHUNK_SIZE)
	{
		*other_chunk = chunk_manager::get_chunk(chunk_context->manager, chunk_context->x + CHUNK_SIZE, chunk_context->y, chunk_context->z);
		x -= CHUNK_SIZE;

		return true;
	}

	if (y < 0)
	{
		*other_chunk = chunk_manager::get_chunk(chunk_context->manager, chunk_context->x, chunk_context->y - CHUNK_SIZE, chunk_context->z);
		y += CHUNK_SIZE;

		return true;
	}

	if (y >= CHUNK_SIZE)
	{
		*other_chunk = chunk_manager::get_chunk(chunk_context->manager, chunk_context->x, chunk_context->y + CHUNK_SIZE, chunk_context->z);
		y -= CHUNK_SIZE;

		return true;
	}

	if (z < 0)
	{
		*other_chunk = chunk_manager::get_chunk(chunk_context->manager, chunk_context->x, chunk_context->y, chunk_context->z - CHUNK_SIZE);
		z += CHUNK_SIZE;

		return true;
	}

	if (z >= CHUNK_SIZE)
	{
		*other_chunk = chunk_manager::get_chunk(chunk_context->manager, chunk_context->x, chunk_context->y, chunk_context->z + CHUNK_SIZE);
		z -= CHUNK_SIZE;

		return true;
	}

	return false;
}

block* chunk::get_block_reference(chunk* chunk_context, int x, int y, int z)
{
	if (!chunk_context->data_generated)
		return nullptr;

	chunk* other_chunk = nullptr;

	if (test_if_out_of_bound(chunk_context, &other_chunk, x, y, z))
	{
		if (other_chunk == nullptr)
			return nullptr;

		return get_block_reference(other_chunk, x, y, z);
	}

	int index = get_block_index(x, y, z);

	return &chunk_context->block_data[index];
}

void chunk::create_block(chunk* chunk_context, int x, int y, int z)
{
	int cx = x + chunk_context->x;
	int cy = y + chunk_context->y;
	int cz = z + chunk_context->z;

	block* working_block = get_block_reference(chunk_context, x, y, z);

	glm::vec3 center = glm::vec3(CHUNK_SIZE / 2);
	glm::vec3 pos = glm::vec3(x, y, z);

	double hieght = (sin(cx / 20.0) + cos(cz / 20.0)) * 10;

	if (cy < hieght)
	{
		working_block->block_id = 1;
	}
	else
	{
		working_block->block_id = 0;
	}

	if (!block::get_is_transparent(working_block))
	{
		chunk_context->blocks_to_generate.push_back(glm::ivec3(x, y, z));
	}
}

static bool chunk_is_ready(chunk* chunk_context)
{
	if (chunk_context == nullptr)
		return false;

	return chunk_context->data_generated;
}

bool chunk::valid_for_mesh_generation(chunk* chunk_context)
{
	int positions[3];

	for (int i = 0; i < 3; ++i)
	{
		positions[0] = chunk_context->x;
		positions[1] = chunk_context->y;
		positions[2] = chunk_context->z;

		for (int sign = -1; sign < 2; sign += 2)
		{
			positions[i] = positions[i] + (sign * CHUNK_SIZE);

			chunk* test_chunk = chunk_manager::get_chunk(chunk_context->manager, positions[0], positions[1], positions[2]);

			if (!chunk_is_ready(test_chunk))
			{
				return false;
			}
		}
	}

	return true;
}

chunk* chunk::allocate_chunk()
{
	return new chunk();
}

void chunk::destroy_allocation(chunk* to_destroy)
{
	destroy(to_destroy);

	delete to_destroy;
}
