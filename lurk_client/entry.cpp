#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "rgame/rgame.h"

#include "lurk_client_context.h"
#include "rgame/font_atlas.h"
#include "rgame/io.h"
#include "rgame/render_texture.h"
#include "rgame/software_text_renderer.h"

lurk_client_context lurk_client;
std::string executable_path;

static void lurk_client_start(render_window* render_window_context, void* arguments)
{
	lurk_client_context::create(&lurk_client, 5025, "isoptera.lcsc.edu", render_window_context, executable_path);
}

static void lurk_client_update(render_window*, void* arguments)
{
	lurk_client_context::draw_boards(&lurk_client);
}

static void lurk_client_destroy(render_window*, void* arguments)
{
	lurk_client_context::destroy(&lurk_client);
}

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	executable_path = std::string(argv[0]);

	render_window client_window;

	render_window::create(&client_window,lurk_client_start,lurk_client_update, lurk_client_destroy, 1024, 900, 60, "LURK");

	render_window::set_resizable(&client_window, false);

	void* arguments[] =
	{
		nullptr,
		nullptr
	};

	render_window::run(&client_window, arguments);

	render_window::destroy(&client_window);
}