#pragma once

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "asset/asset_collection.h"

struct render_window;
struct actor_mario;
struct io;

struct game_scene
{
	render_window*		main_window;
	io*					io_context;
	asset_collection	asset_collection_context;

	static void			create(game_scene* result);
	static void			destroy(game_scene* game_scene_context);
};

#endif