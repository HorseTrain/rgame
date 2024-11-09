#include "maze_data.h"
#include "maze.h"

void maze_cell::find_neighbors(maze_cell* cell)
{
	cell->neighbors[0] = maze::get_cell(cell->maze_context, cell->x, cell->y - 1);
	cell->neighbors[1] = maze::get_cell(cell->maze_context, cell->x + 1, cell->y);
	cell->neighbors[2] = maze::get_cell(cell->maze_context, cell->x, cell->y + 1);
	cell->neighbors[3] = maze::get_cell(cell->maze_context, cell->x - 1, cell->y);
}

bool maze_cell::is_dead_end(maze_cell* cell)
{
	for (int i = 0; i < NEIGHBOR_COUNT; ++i)
	{
		if (cell->neighbors[i] == nullptr)
			continue;

		if (!cell->neighbors[i]->visited)
		{
			return false;
		}
	}

	return true;
}
