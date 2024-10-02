#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>

#include "rgame/render_window.h"
#include "game/game_scene.h"
#include "game/game_context.h"

game_context main_game;

static void game_start(render_window* window, void* source_arguments)
{
	void** arguments = (void**)source_arguments;

	game_context::create(&main_game, arguments);
}

static void game_loop(render_window* window, void* arguments)
{
	game_context::update(&main_game);
}

static void game_end(render_window* window, void* arguments)
{
	game_context::destroy(&main_game);
}

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	render_window main_window;

	render_window::create(&main_window, game_start, game_loop, game_end, 500, 500, 60, "MARIO RAHH");

	game_context_entry_command test_command;

	test_command.command = GAME_ENTRY_INIT_LEVEL;
	*(int*)test_command.argumens = -1;

	void* arguments[] = {
		(void*)argc,
		argv,
		&main_window,
		&test_command
	};

	render_window::run(&main_window, arguments);

	render_window::destroy(&main_window);
}