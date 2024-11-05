#include "maze_level.h"
#include "mario/level.h"
#include "mario/game_object.h"
#include "maze.h"

void maze_level::create(maze_level** maze_level_context,level* level_context)
{
	maze_level* result = new maze_level();

	result->level_context = level_context;

	result->maze_context = game_object::create_game_object<maze>(level_context);

	maze::create(result->maze_context, result);

	*maze_level_context = result;
}

void maze_level::destroy(maze_level* maze_level_context)
{
	delete maze_level_context;
}
