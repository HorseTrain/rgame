#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "rgame/rgame.h"

#include "lurk_client_context.h"

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	lurk_client_context lurk_client;

	lurk_client_context::create(&lurk_client, 5025, "isoptera.lcsc.edu");

	lurk_client_context::run(&lurk_client);

	lurk_client_context::destroy(&lurk_client);

	/*
	render_window client_window;

	render_window::create(&client_window, nullptr, 500, 500, 60, "LURK");

	render_window::set_resizable(&client_window, false);

	void* arguments[] =
	{
		nullptr,
		nullptr
	};

	render_window::run(&client_window, arguments);

	render_window::destroy(&client_window);
	*/
}