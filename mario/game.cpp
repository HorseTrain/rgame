#include "game.h"

#include "mario.h"

#include "rgame/shader_compiler.h"

#include "render/model_loader.h"
#include "rgame/render_texture.h"

static render_surface* create_debug_surface(game* game_context, std::string source_path, std::string name)
{
	render_surface* result_render_surface = lifetime_memory_manager::allocate<render_surface>(&game_context->memory, name + "_surface");
	render_shader* result_render_shader = lifetime_memory_manager::allocate<render_shader>(&game_context->memory, name + "_shader");

	get_shader_from_path(result_render_shader, source_path, game_context->io_context);

	render_surface::create(result_render_surface, result_render_shader);

	return result_render_surface;
}

static void init_debug_assets(game* game_context)
{
	static_render_mesh* debug_mesh;

	debug_mesh = lifetime_memory_manager::allocate<static_render_mesh>(&game_context->memory, "debug_triangle");
	static_render_mesh::create_debug_cube(debug_mesh, 1);

	debug_mesh = lifetime_memory_manager::allocate<static_render_mesh>(&game_context->memory, "debug_plane");
	static_render_mesh::create_debug_plane(debug_mesh, 100);

	render_surface* debug_surface = create_debug_surface(game_context, "shaders/unskinned_actor/", "debug_surface");
	render_surface* debug_texture = create_debug_surface(game_context, "shaders/debug_texture/", "debug_texture");
}

static void game_start(render_window*,void* arguments)
{
	game* game_context = (game*)((uint64_t*)arguments)[0];
}

static void game_update(render_window*, void* arguments)
{
	game* game_context = (game*)((uint64_t*)arguments)[0];

	input_manager::update(game_context->input_manager_context);

	if (game_context->level_context != nullptr)
	{
		level::update(game_context->level_context);
	}
}

static void game_end(render_window*, void* arguments)
{
	game* game_context = (game*)((uint64_t*)arguments)[0];
}

static void load_debug_level(game* game_context)
{
	//TODO:

	throw 0;
}

static void load_level(game* game_context, uint64_t level_index, uint64_t level_destroy)
{
	if (level_index == -1)
	{
		load_debug_level(game_context);
	}
	else
	{
		void (*create_function)(void**, level*);

		create_function = (void(*)(void**, level*))level_index;

		game::create_empty_level(game_context);

		create_function((void**)&game_context->level_context->level_buffer, game_context->level_context);

		game_context->level_context->level_buffer_destroy = (void*)level_destroy;
	}
}

static void init_io(game* game_context, std::string executable_path)
{
	std::string executable_directory;
	io::get_parent_directory(&executable_directory, executable_path);

	io::create(game_context->io_context, executable_directory + "assets/");
}

void game::send_command(game* game_context, game_command command)
{
	switch (command.command)
	{
	case game_load_level: load_level(game_context,command.arguments[0], command.arguments[1]); break;
	case game_close: game_context->window_context->force_close = true; break;
	default: assert(false); throw 0;
	}
}

void game::create(game* game_context, std::string executable_path, std::string window_name)
{
	lifetime_memory_manager::create(&game_context->memory);

	game_context->window_context = lifetime_memory_manager::allocate<render_window>(&game_context->memory, "render_window_context");
	game_context->io_context = lifetime_memory_manager::allocate<io>(&game_context->memory, "io_context");
	game_context->input_manager_context = lifetime_memory_manager::allocate<input_manager>(&game_context->memory, "input_manager_context");

	init_io(game_context, executable_path);

	render_window::create(game_context->window_context, game_start, game_update, game_end, 500, 500, 60, window_name);
	input_manager::create(game_context->input_manager_context, game_context->window_context);

	game_context->level_context = nullptr;

	init_debug_assets(game_context);
}

void game::run(game* game_context)
{
	void* arguments[] = { game_context };

	render_window::run(game_context->window_context, arguments);
}

void game::destroy(game* game_context)
{
	destroy_current_level(game_context);

	render_window::destroy(game_context->window_context);

	lifetime_memory_manager::destroy(&game_context->memory);
}

void game::destroy_current_level(game* game_context)
{
	level* level_context = game_context->level_context;

	if (level_context == nullptr)
		return;

	if (level_context->level_buffer_destroy != nullptr)
	{
		((void(*)(void*))level_context->level_buffer_destroy)(level_context->level_buffer);
	}

	level::destroy(level_context);
	delete level_context;

	game_context->level_context = nullptr;
}

void game::create_empty_level(game* game_context)
{
	destroy_current_level(game_context);

	game_context->level_context = new level();

	level::create(game_context->level_context, game_context);
}
