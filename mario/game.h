#ifndef GAME_H
#define GAME_H

#include "game_command.h"
#include "rgame/lifetime_memory_manager.h"

struct render_window;
struct io;
struct input_manager;
struct level;

enum game_mode
{
	inside_level,
};

struct game
{
	lifetime_memory_manager memory;

	render_window*			window_context;
	io*						io_context;
	input_manager*			input_manager_context;

	level*					level_context;

	static void				send_command(game* game_context, game_command command);
	static void				create(game* game_context, std::string executable_path, std::string window_name = "");
	static void				run(game* game_context);
	static void				destroy(game* game_context);
	static void				destroy_current_level(game* game_context);
	static void				create_empty_level(game* game_context);
};

#endif