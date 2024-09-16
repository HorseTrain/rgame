#include "world.h"

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
