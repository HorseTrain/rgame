#pragma once

#ifndef MAZE_CELL_H
#define MAZE_CELL_H

struct maze;

struct maze_cell
{
	maze*	maze_context;
	bool	doors[4];

	//0 north
	//1 east
	//2 south
	//3 west
};

#endif