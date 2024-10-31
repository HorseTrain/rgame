#include "game_command.h"

game_command game_command::create_command(game_command_header header, std::vector<uint64_t> arguments)
{
	game_command result;

	result.command = header;
	result.arguments = arguments;

	return result;
}
