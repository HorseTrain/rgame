#include "world.h"
#include "chunk.h"

void world::create(world* world_context, game* game_context, uint64_t seed)
{
	world_context->seed = seed;
	world_context->game_context = game_context;

	player::create(&world_context->main_player, world_context);

	chunk_manager::create(&world_context->chunk_manager_context, world_context);
	render_camera::create_empty(&world_context->main_camera, 60, 1, 1000);
}

void world::destroy(world* world_context)
{
	chunk_manager::destroy(&world_context->chunk_manager_context);
	player::destroy(&world_context->main_player);
}

void world::update(world* world_context)
{
	player::update(&world_context->main_player);
	chunk_manager::update(&world_context->chunk_manager_context);
}

void world::get_block(world* world_context, glm::ivec3 world_position, block** result_block, chunk** result_chunk)
{
	glm::ivec3 chunk_location;
	glm::ivec3 chunk_position;

	for (int i = 0; i < 3; ++i)
	{
		int axis = world_position[i];

		bool negate = axis < 0;

		axis /= CUBE_CHUNK_SIZE;

		if (negate)
			axis--;

		chunk_location[i] = axis * CUBE_CHUNK_SIZE;

		chunk_position[i] = world_position[i] % CUBE_CHUNK_SIZE;

		if (negate)
		{
			chunk_position[i] += CUBE_CHUNK_SIZE - 1;
		}
	}

	*result_chunk = chunk_store::create_or_get_chunk(&world_context->chunk_manager_context.chunks, chunk_location);
	*result_block = chunk::get_block_reference(*result_chunk, chunk_position);
}
