#pragma once

#ifndef LEVEL_SCENE_H
#define LEVEL_SCENE_H

#include <vector>

#include "asset/asset_collection.h"
#include "rgame/render_camera.h"
#include <unordered_set>

struct game_context;
struct actor;

struct level_scene
{
	int							level_id;
	game_context*				game_context_context;
	asset_collection			asset_collection_context;
	std::unordered_set<actor*>	actors;

	render_camera				main_camera;

	static void			create(level_scene* result,game_context* game_context_context, int level_id);
	static void			update(game_context* game_context_context);
	static void			destroy(game_context* game_context_context);

	static actor*		create_empty_actor(level_scene* scene);
	static void			destroy_actor(actor* to_destroy, bool remove_from_collection = true);
	static void			update_actor(actor* to_update);
};

#endif