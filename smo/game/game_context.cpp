#include "game_context.h"
#include "asset/shader_loader.h"
#include "render/render_model.h"

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

	std::string asset_path = std::string(entry_arguments[0]);
	
	to_create->render_window_context = (render_window*)arguments[3];

	init_game_context_io(to_create, asset_path);
	init_game_assets(to_create);
}

void game_context::update(game_context* to_update)
{
	static_render_mesh* test_mesh = asset_collection::get_asset_by_name<static_render_mesh>(&to_update->game_assets,"debug_triangle");
	render_shader* rest_shader = asset_collection::get_asset_by_name<render_shader>(&to_update->game_assets, "unskinned_actor");

	render_shader::use(rest_shader);
	static_render_mesh::draw(test_mesh);
}

void game_context::destroy(game_context* to_destroy)
{
	asset_collection::destroy(&to_destroy->game_assets);
}
