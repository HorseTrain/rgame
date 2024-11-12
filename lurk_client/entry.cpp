#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "rgame/rgame.h"


int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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
}