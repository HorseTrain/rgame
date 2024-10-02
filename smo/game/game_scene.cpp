#include "game_scene.h"

void game_scene::create(game_scene* result)
{
	asset_collection::create(&result->asset_collection_context);
}

void game_scene::destroy(game_scene* game_scene_context)
{
	asset_collection::destroy(&game_scene_context->asset_collection_context);
}
