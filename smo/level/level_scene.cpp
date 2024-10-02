#include "level_scene.h"
#include "game/game_context.h"
#include "actor/actor.h"

#include "actor/actor_mario.h"

static void load_debug_scene(level_scene* level_scene_context)
{
	render_camera::create_empty(&level_scene_context->main_camera, 60, 1, 1000);


	
}

void level_scene::create(level_scene* result, game_context* game_context_context, int level_id)
{
	result->game_context_context = game_context_context;
	result->level_id = level_id;

	asset_collection::create(&result->asset_collection_context);

	if (level_id == -1)
	{
		load_debug_scene(result);
	}
	else
	{
		throw 0;
	}
}

static void level_scene_destroy(level_scene* scene)
{
	for (auto i : scene->actors)
	{
		level_scene::destroy_actor(i, false);
	}

	asset_collection::destroy(&scene->asset_collection_context);
}

static void level_scene_update(level_scene* scene)
{
	for (auto i : scene->actors)
	{
		level_scene::update_actor(i);
	}
}

void level_scene::destroy(game_context* game_context_context)
{
	level_scene* working_scene = (level_scene*)game_context_context->current_game;

	level_scene_destroy(working_scene);

	delete working_scene;
}

actor* level_scene::create_empty_actor(level_scene* scene)
{
	actor* result = new actor();

	result->level_scene_context = scene;
	result->game_context_context = scene->game_context_context;

	result->started = false;

	result->actor_create = nullptr;
	result->actor_update = nullptr;
	result->actor_destroy = nullptr;

	scene->actors.insert(result);

	return result;
}

void level_scene::destroy_actor(actor* to_destroy, bool remove_from_collection)
{
	if (to_destroy->actor_destroy != nullptr)
	{
		to_destroy->actor_destroy(to_destroy);
	}

	if (remove_from_collection)
	{
		to_destroy->level_scene_context->actors.erase(to_destroy);
	}

	delete to_destroy;
}

void level_scene::update_actor(actor* to_update)
{
	if (to_update->actor_create != nullptr && !to_update->started)
	{
		to_update->actor_create(to_update);

		to_update->started = true;
	}

	if (to_update->actor_update != nullptr)
	{
		to_update->actor_update(to_update);
	}
}

void level_scene::update(game_context* game_context_context)
{
	level_scene* working_scene = (level_scene*)game_context_context->current_game;

	level_scene_update(working_scene);
}