#include "maze_level.h"
#include "mario/level.h"

void maze_level::create(maze_level** maze_level_context,level* level_context)
{
	maze_level* result = new maze_level();

	result->level_context = level_context;

	*maze_level_context = result;
}

void maze_level::destroy(maze_level* maze_level_context)
{
	delete maze_level_context;
}
