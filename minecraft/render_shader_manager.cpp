#include "render_shader_manager.h"
#include "rgame/render_shader.h"
#include "rgame/io.h"
#include "graphics_asset_manager.h"
#include "rgame/render_window.h"
#include "assert.h"

void create_render_shader(render_shader** result, graphics_asset_manager* manager, std::string name)
{
	render_shader* working_result;

	working_result = new render_shader();
	render_shader::create(working_result);

	graphics_asset* asset_to_append;
	graphics_asset::create(&asset_to_append, manager,minecraft_shader, name, working_result);

	*result = working_result;
}

static void load_shader(render_shader* result, io* io_context,std::string base_path, std::string shader_name, uint32_t gl_type)
{
	std::string possible_shader_path = io::get_path(io_context, base_path + "\\" + shader_name);

	if (!io::file_exists(possible_shader_path))
	{
		return;
	}

	char* shader_string_buffer;
	uint64_t shader_size;

	io::io_read_entire_file(io_context, &shader_string_buffer, &shader_size, possible_shader_path);

	std::string shader_source_string = std::string(shader_string_buffer, shader_size);

	free(shader_string_buffer);

	render_shader::append_shader_source(result, gl_type, shader_source_string);
}

void load_shader_from_path(render_shader** resultm, graphics_asset_manager* manager, std::string name, io* io_context, std::string path)
{
	path = io::get_path(io_context, path);
	create_render_shader(resultm, manager, name);

	render_shader* working_result = *resultm;

	load_shader(working_result, io_context, path, "vertex.glsl", GL_VERTEX_SHADER);
	load_shader(working_result, io_context, path, "fragment.glsl", GL_FRAGMENT_SHADER);

	render_shader::compile(working_result);
}
