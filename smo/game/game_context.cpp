#include "game_context.h"
#include "asset/shader_loader.h"
#include "render/render_model.h"
#include "level/level_scene.h"

#include "rgame/render_shader.h"
#include "rgame/io.h"
#include "rgame/string_tools.h"

#include <string>
#include <vector>

static void load_universal_shaders(game_context* game_context_context)
{
	load_shader("unskinned_actor", "shaders/unskinned_actor/", &game_context_context->game_assets, &game_context_context->io_context);
}

static void load_universal_meshes(game_context* game_context_context)
{
	static_render_mesh* triangle;
	static_render_mesh::create_mesh_with_asset_collection(&triangle, &game_context_context->game_assets, "debug_triangle");

	static_render_mesh::create_triangle(triangle);
}

static void init_game_context_io(game_context* game_context_context, std::string asset_path)
{
	string_get_directory(&asset_path, asset_path);

	asset_path += "assets/";

	io::create(&game_context_context->io_context, asset_path);
}

static void init_game_assets(game_context* game_context_context)
{
	load_universal_shaders(game_context_context);
	load_universal_meshes(game_context_context);
}

void game_context::create(game_context* to_create, void** arguments)
{
	int entry_argument_count = (int)arguments[0];
	char** entry_arguments = (char**)arguments[1];
	to_create->render_window_context = (render_window*)arguments[2];
	game_context_entry_command* entry_command = (game_context_entry_command*)arguments[3];

	std::string asset_path = std::string(entry_arguments[0]);

	init_game_context_io(to_create, asset_path);
	init_game_assets(to_create);

	input_manager::create(&to_create->input, to_create->render_window_context);

	to_create->current_game = nullptr;

	process_entry_command(to_create, entry_command);
}

void game_context::update(game_context* to_update)
{
	input_manager::update(&to_update->input);

	if (to_update->current_game != nullptr)
	{
		to_update->current_update_game(to_update);
	}
}

void game_context::destroy(game_context* to_destroy)
{
	destroy_current_game_context(to_destroy);

	asset_collection::destroy(&to_destroy->game_assets);
}

void game_context::process_entry_command(game_context* game_context_context, game_context_entry_command* entry_command)
{
	destroy_current_game_context(game_context_context);

	switch (entry_command->command)
	{
	case GAME_ENTRY_INIT_LEVEL:
	{
		int level_id = *(int*)entry_command->argumens;

		level_scene* working_scene = new level_scene();

		level_scene::create(working_scene, game_context_context, level_id);

		game_context_context->current_game = working_scene;

		game_context_context->current_update_game = level_scene::update;
		game_context_context->current_destroy_game = level_scene::destroy;
		
	}; break;
	default: assert(false); throw 0;
	}
}

void game_context::destroy_current_game_context(game_context* game_context_context)
{
	if (game_context_context->current_game == nullptr)
		return;

	game_context_context->current_destroy_game(game_context_context);
}
