#include <iostream>

#include "world.h"

#include "rgame/render_window.h"
#include "rgame/game_scene.h"

int main()
{
	render_window main_window;
	game_scene raw_scene;

	render_window::create(&main_window, world::scene_world_loop, 600, 600, 60, "Minecraft");
	game_scene::create(&raw_scene, &main_window);


	world main_world;
	world::create(&main_world, &raw_scene);

	uint64_t arguments[] = { (uint64_t)&main_world };

	render_window::run(&main_window, arguments);

	world::destroy(&main_world);

	render_window::destroy(&main_window);
}