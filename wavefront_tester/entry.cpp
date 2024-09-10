#include <iostream>

#include "rgame/render_window.h"
#include "obj_model_scene.h"

#include "rgame/game_scene.h"

obj_model_scene model_scene;

static void update_model_scene(render_window* window, uint64_t)
{
	obj_model_scene::update(&model_scene);
}

static void print_help()
{
	std::cout << "usage" << std::endl;
	std::cout << "\twavefront_tester [obj path]" << std::endl << std::endl;
	std::cout << "\tMove camera with WASD. Mouse movements will be added soon." << std::endl;
}

int main(int argc, char** argv)
{
	std::string path = "C:\\Users\\raymo\\source\\repos\\assets\\amy.obj";

	if (argc != 2)
	{
		print_help();

		//return -1;
	}
	else
	{
		path = argv[1];
	}

	render_window	main_window;
	game_scene		raw_scene;

	render_window::create(&main_window, update_model_scene, 500, 500, 60.0f,"wavefront viewer");
	game_scene::create(&raw_scene, &main_window);

	obj_model_scene::create(&model_scene, &raw_scene, path);
	render_window::run(&main_window);

	obj_model_scene::destroy(&model_scene);
}