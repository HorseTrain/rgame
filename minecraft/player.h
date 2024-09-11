#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "rgame/transform.h"

struct game;

struct player
{
	game*		game_context;
	transform	transform_context;

	static void create(player* player_context, game* game_context);
	static void destroy(player* player_context);
	static void update(player* player_context);
};

#endif