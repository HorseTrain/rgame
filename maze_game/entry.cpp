#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "rgame/rgame.h"
#include "game.h"

#include "maze_level.h"

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	game game;

	game::create(&game, argv[0]);

	game::send_command(&game, game_command::create_command(game_command_header::game_load_level, { (uint64_t)maze_level::create, (uint64_t)maze_level::destroy }));

	game::run(&game);

	game::destroy(&game);
}