#include "shader_loader.h"
#include "asset/asset_collection.h"
#include "rgame/render_shader.h"
#include "rgame/io.h"
#include "rgame/render_window.h"
#include "rgame/shader_compiler.h"

static void create_shader_asset(asset_instance* asset_instance_context, void** arguments)
{
	std::string* source_path = (std::string*)arguments[0];
	io* io_context = (io*)arguments[1];

	render_shader* result_shader = new render_shader();

	render_shader::create(result_shader);

	asset_instance_context->buffer = result_shader;
}

static void destroy_shader_asset(asset_instance* asset_instance_context, void** arguments)
{
	render_shader* working_shader = (render_shader*)asset_instance_context->buffer;

	render_shader::destroy(working_shader);

	delete working_shader;
}

static void create_shader_instance(asset_instance** result_instance,std::string shader_name, std::string source_path, asset_collection* asset_collection_context, io* io_context)
{
	void* shader_creation_arguments[] = {

		&source_path,
		io_context
	};

	asset_collection::create_asset_instance(result_instance, asset_collection_context, create_shader_asset, destroy_shader_asset,shader_name);
	asset_instance::create_asset(*result_instance, shader_creation_arguments);
}

static void append_shader_if_exists(render_shader* result, std::string source_path, std::string shader_path,io* io_context, int type)
{
	shader_path = source_path + shader_path;

	if (!io::file_exists(shader_path))
		return;

	std::string shader_code;
	compile_shader_source(&shader_code, shader_path, io_context);

	render_shader::append_shader_source(result, type, shader_code);
}

void load_shader(std::string shader_name, std::string source_path, asset_collection* asset_collection_context, io* io_context)
{
	asset_instance* shader_instance;
	create_shader_instance(&shader_instance, shader_name, source_path, asset_collection_context, io_context);

	render_shader* result_shader = (render_shader*)shader_instance->buffer;

	source_path = io::get_path(io_context, source_path);

	append_shader_if_exists(result_shader, source_path, "vertex.glsl", io_context, GL_VERTEX_SHADER);
	append_shader_if_exists(result_shader, source_path, "fragment.glsl", io_context, GL_FRAGMENT_SHADER);

	render_shader::compile(result_shader);
}
