#pragma once

#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "rgame/io.h"
#include "rgame/input_manager.h"
#include "graphics_asset_manager.h"
#include <string>

struct render_window;

struct game
{
	render_window*			window_context;
	io						io_context;
	graphics_asset_manager	graphics_asset_manager_context;
	input_manager			input_manager_context;

	int						mouse_toggle;

	static void				create(game* game_context, render_window* render_window_context, std::string asset_path);
	static void				destroy(game* game_context);
	static void				update(game* game_context);
};

#endif