#pragma once

#ifndef GAME_COMMAND_H
#define GAME_COMMAND_H

#include <vector>

enum game_command_header
{
	game_load_level,
	game_close
};

struct game_command
{
	game_command_header command;
	std::vector<int>	arguments;

	static game_command create_command(game_command_header header, std::vector<int> arguments);
};

#endif