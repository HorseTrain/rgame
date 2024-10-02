#pragma once

#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "rgame/io.h"
#include "asset/asset_collection.h"

struct render_window;

struct game_context
{
	render_window*		render_window_context;
	io					io_context;
	asset_collection	game_assets;

	static void			create(game_context* to_create, void** arguments);
	static void			update(game_context* to_update);
	static void			destroy(game_context* to_destroy);
};

#endif