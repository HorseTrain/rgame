#include "world.h"
#include "chunk_manager.h"
#include "rgame/game_scene.h"
#include "rgame/render_window.h"
#include "rgame/render_mesh.h"
#include <iostream>

static void create_world_input_manager(world* world_context)
{
	input_manager* input_manager_context = &world_context->input_manager_context;

	input_manager::create(input_manager_context, world_context->raw_scene->window);

	input_manager::register_key(input_manager_context, GLFW_KEY_W);
	input_manager::register_key(input_manager_context, GLFW_KEY_A);
	input_manager::register_key(input_manager_context, GLFW_KEY_S);
	input_manager::register_key(input_manager_context, GLFW_KEY_D);

	input_manager::register_key(input_manager_context, GLFW_KEY_LEFT_SHIFT);
	input_manager::register_key(input_manager_context, GLFW_KEY_SPACE);
}

void world::create(world* result, game_scene* raw_scene)
{
	result->raw_scene = raw_scene;
	result->world_started = false;

	chunk_manager::create(&result->chunk_manager_context, result);
	world_graphics_manager::create(&result->world_graphics_manager_context, result);
	player::create(&result->main_player, result);

	create_world_input_manager(result);
}

void world::destroy(world* to_destroy)
{
	chunk_manager::destroy(&to_destroy->chunk_manager_context);
	player::destroy(&to_destroy->main_player);
	world_graphics_manager::destroy(&to_destroy->world_graphics_manager_context);
}

void world::world_start(world* ctx)
{
}

void world::world_update(world* ctx)
{
	input_manager::update(&ctx->input_manager_context);
	player::update(&ctx->main_player);

	chunk_manager::chunk_manager_update(&ctx->chunk_manager_context);
}

void world::scene_world_loop(render_window* window, uint64_t frame_time, void* arguments)
{
	uint64_t* args = (uint64_t*)arguments;

	world* working_world = (world*)args[0];

	if (!working_world->world_started)
	{
		world_start(working_world);

		working_world->world_started = true;
	}

	world_update(working_world);
}
