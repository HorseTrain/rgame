#include <iostream>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "rgame/render_window.h"
#include "rgame/render_shader.h"
#include "rgame/io.h"
#include "rgame/render_texture.h"

#include "render_shader_manager.h"
#include "render_texture_manager.h"
#include "game.h"
#include "world.h"

bool started = false;

game main_game;
world main_world;

void loop(render_window* window, void* arguments)
{
	uint64_t* loop_arguments = (uint64_t*)arguments;

	if (!started)
	{
		std::string asset_path = std::string((char*)loop_arguments[0]);
		io::get_parent_directory(&asset_path, asset_path);
		asset_path += "/assets";

		game::create(&main_game, window, asset_path);
		world::create(&main_world, &main_game, 0);

		render_shader* tmp_shader;
		render_texture* tmp_texture;
		load_shader_from_path(&tmp_shader, &main_game.graphics_asset_manager_context, "default_chunk_shader", &main_game.io_context, "shaders/default_chunk/");
		load_texture_from_path(&tmp_texture, &main_game.graphics_asset_manager_context, "default_chunk_texture", &main_game.io_context, "t_atlas.gif");

		started = true;
	}

	game::update(&main_game);
	world::update(&main_world);
}

#include "rgame/string_tools.h"
#include "io.h"

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	uint64_t* game_arguments[] = {
		(uint64_t*)argv[0]
	};

	render_window test_window;

	render_window::create(&test_window, loop, 500, 500, 60, "minecraft");
	render_window::run(&test_window, game_arguments);

	game::destroy(&main_game);
	world::destroy(&main_world);

	render_window::destroy(&test_window);
}