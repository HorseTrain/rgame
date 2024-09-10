#include "world_graphics_manager.h"
#include "world.h"
#include "rgame/render_window.h"

static void create_default_shaders(world_graphics_manager* world_graphics_manager_context)
{
	const char* default_vertex = R"(#version 330 core

in vec3 i_position;
in vec3 i_normal;
in vec2 i_uv;
in vec4 i_color;

out vec3 position;
out vec3 normal;
out vec2 uv;
out vec4 color;

uniform mat4 camera_transform;
uniform mat4 object_transform;

vec3 transform_vec3(mat4 transformation, vec3 point)
{
	return (transformation * vec4(point, 1)).xyz;
}

vec3 transform_direction(mat4 transformation, vec3 point)
{
	return transform_vec3(transpose(inverse(transformation)), point);
}

void main()
{
	position = transform_vec3(object_transform, i_position);
	normal = transform_direction(object_transform, i_normal);

	uv = i_uv;
	color = i_color;

	gl_Position = camera_transform * vec4(position, 1);
})";

	const char* default_fragment = R"(#version 330 core

in vec3 position;
in vec3 normal;
in vec2 uv;
in vec4 color;

out vec4 frag_color;

void main()
{
	frag_color = vec4(normal, 1);
}

)";

	render_shader* result = world_graphics_manager::create_render_shader(world_graphics_manager_context, "default");

	render_shader::append_shader_source(result, GL_VERTEX_SHADER, default_vertex);
	render_shader::append_shader_source(result, GL_FRAGMENT_SHADER, default_fragment);
	render_shader::compile(result);
}

void world_graphics_manager::create(world_graphics_manager* result, world* world_context)
{
	result->world_context = world_context;
	result->loaded_shaders = std::map<std::string, render_shader*>();

	create_default_shaders(result);
}

void world_graphics_manager::destroy(world_graphics_manager* world_graphics_manager_context)
{
	for (auto i = world_graphics_manager_context->loaded_shaders.begin(); i != world_graphics_manager_context->loaded_shaders.end(); ++i)
	{
		render_shader* working_shader = i->second;

		render_shader::destroy(working_shader);
		delete working_shader;
	}
}

void world_graphics_manager::load_shader(world_graphics_manager* world_graphics_manager_context, std::string name, std::string directory_path)
{

}

render_shader* world_graphics_manager::create_render_shader(world_graphics_manager* world_graphics_manager_context, std::string name)
{
	render_shader* result = new render_shader();

	render_shader::create(result);

	if (world_graphics_manager_context->loaded_shaders.find(name) != world_graphics_manager_context->loaded_shaders.end())
	{
		assert(false);

		throw 0;
	}

	world_graphics_manager_context->loaded_shaders[name] = result;

	return result;
}
