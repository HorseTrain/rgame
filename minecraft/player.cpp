#include "player.h"
#include "rgame/transform.h"

void player::create(player* player_context, game* game_context)
{
	player_context->game_context = game_context;

	transform::create_identity(&player_context->transform_context);
}
