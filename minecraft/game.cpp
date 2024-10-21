#include "game.h"
#include "game.h"
#include "game.h"
#include "rgame/render_window.h"

void game::create(game* game_context, render_window* render_window_context, std::string asset_path)
{
	game_context->window_context = render_window_context;

	graphics_asset_manager::create(&game_context->graphics_asset_manager_context);

	io::create(&game_context->io_context, asset_path);
	input_manager::create(&game_context->input_manager_context, render_window_context);

	game_context->mouse_toggle = -1;
}

void game::destroy(game* game_context)
{
	graphics_asset_manager::destroy(&game_context->graphics_asset_manager_context);
}

static void manage_game_mouse(game* game_context)
{
	input_manager* input_manager_context = &game_context->input_manager_context;

	if (input_manager::get_key_pressed(input_manager_context, GLFW_KEY_ESCAPE))
	{
		game_context->mouse_toggle *= -1;

		if (game_context->mouse_toggle == 1)
		{
			input_manager::lock_mouse(input_manager_context);
		}
		else
		{
			input_manager::unlock_mouse(input_manager_context);
		}
	}
}

void game::update(game* game_context)
{
	input_manager::update(&game_context->input_manager_context);

	manage_game_mouse(game_context);
}
