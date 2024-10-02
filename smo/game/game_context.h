#pragma once

#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "rgame/io.h"
#include "rgame/input_manager.h"
#include "asset/asset_collection.h"

struct render_window;

#define GAME_ENTRY_INIT_LEVEL 0

struct game_context;

typedef void(*game_instance_function)(game_context* game_context_context);

struct game_context_entry_command
{
	int		command;
	uint8_t argumens[64];
};

struct game_context
{
	render_window*		render_window_context;
	io					io_context;
	input_manager		input;
	asset_collection	game_assets;

	void*					current_game;
	game_instance_function	current_destroy_game;
	game_instance_function	current_update_game;

	static void			create(game_context* to_create, void** arguments);
	static void			update(game_context* to_update);
	static void			destroy(game_context* to_destroy);
	static void			process_entry_command(game_context* game_context_context,game_context_entry_command* entry_command);
	static void			destroy_current_game_context(game_context* game_context_context);
};

#endif