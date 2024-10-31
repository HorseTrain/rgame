#include "game.h"

#include "mario.h"

#include "rgame/shader_compiler.h"

#include "render/model_loader.h"
#include "rgame/render_texture.h"

static void init_debug_assets(game* game_context)
{
	static_render_mesh* debug_mesh;

	debug_mesh = lifetime_memory_manager::allocate<static_render_mesh>(&game_context->memory, "debug_triangle");
	static_render_mesh::create_debug_cube(debug_mesh, 1);

	debug_mesh = lifetime_memory_manager::allocate<static_render_mesh>(&game_context->memory, "debug_plane");
	static_render_mesh::create_debug_plane(debug_mesh, 100);

	render_surface* debug_surface = lifetime_memory_manager::allocate<render_surface>(&game_context->memory, "debug_surface");
	render_shader* debug_shader = lifetime_memory_manager::allocate<render_shader>(&game_context->memory, "debug_shader");

	render_shader::create(debug_shader);

	std::string vertex_source;
	std::string fragment_source;

	compile_shader_source(&vertex_source, "shaders/unskinned_actor/vertex.glsl", game_context->io_context);
	compile_shader_source(&fragment_source, "shaders/unskinned_actor/fragment.glsl", game_context->io_context);

	render_shader::append_shader_source(debug_shader, GL_VERTEX_SHADER, vertex_source);
	render_shader::append_shader_source(debug_shader, GL_FRAGMENT_SHADER, fragment_source);

	render_shader::compile(debug_shader);

	render_surface::create(debug_surface, debug_shader);

	render_surface::set_data<glm::mat4>(debug_surface, "camera_transform", glm::mat4(1), render_surface_data_type::mat4x4);
	render_surface::set_data<glm::mat4>(debug_surface, "object_transform", glm::mat4(1), render_surface_data_type::mat4x4);
}

static void game_start(render_window*,void* arguments)
{
	game* game_context = (game*)((uint64_t*)arguments)[0];

	init_debug_assets(game_context);
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
		
}

static void load_level(game* game_context, int level_index)
{
	if (level_index == -1)
	{
		load_debug_level(game_context);
	}
	else
	{
		assert(false);
		throw 0;
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
	case game_load_level: load_level(game_context,command.arguments[0]); break;
	case game_close: game_context->window_context->force_close = true; break;
	default: assert(false); throw 0;
	}
}

void game::create(game* game_context, std::string executable_path)
{
	lifetime_memory_manager::create(&game_context->memory);

	game_context->window_context = lifetime_memory_manager::allocate<render_window>(&game_context->memory, "render_window_context");
	game_context->io_context = lifetime_memory_manager::allocate<io>(&game_context->memory, "io_context");
	game_context->input_manager_context = lifetime_memory_manager::allocate<input_manager>(&game_context->memory, "input_manager_context");

	init_io(game_context, executable_path);

	render_window::create(game_context->window_context, game_start, game_update, game_end, 500, 500, 60, "MARIO");
	input_manager::create(game_context->input_manager_context, game_context->window_context);

	game_context->level_context = nullptr;
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
	if (game_context->level_context == nullptr)
		return;

	level::destroy(game_context->level_context);
	delete game_context->level_context;

	game_context->level_context = nullptr;
}

void game::create_empty_level(game* game_context)
{
	destroy_current_level(game_context);

	game_context->level_context = new level();
	level::create(game_context->level_context, game_context);
}