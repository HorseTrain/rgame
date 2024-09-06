#include "game_object.h"

void game_object::create_empty(game_object* result)
{
	result->started = false;

	result->object_start = nullptr;
	result->object_update = nullptr;
	result->object_draw = nullptr;
}
