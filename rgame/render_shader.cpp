#include <iostream>

#include "render_shader.h"
#include "GL/glew.h"
#include "assert.h"

void render_shader::create(render_shader* result)
{
	result->handle = -1;
	result->sources = std::vector<render_shader_source>();
	result->uniform_locations = nullptr;
}

render_shader_source render_shader_source::create(uint32_t type, std::string source)
{
	render_shader_source result;

	result.shader_type = type;
	result.source = source;

	return result;
}

void render_shader::append_shader_source(render_shader* program, uint32_t type, std::string source_code)
{
	render_shader_source working_source = { -1, type, source_code };

	program->sources.push_back(working_source);
}

void render_shader::compile(render_shader* program)
{
	if (program->handle != -1)
		return;

	program->uniform_locations = new std::map<std::string, int>();

	render_shader::destroy(program);

	program->handle = glCreateProgram();

	for (int i = 0; i < program->sources.size(); ++i)
	{
		render_shader_source* working_source = &program->sources[i];

		render_shader_source::compile(working_source);

		glAttachShader(program->handle, working_source->handle);
	}

	glLinkProgram(program->handle);
	glValidateProgram(program->handle);
}

//There is a memory leak among us
void render_shader::destroy(render_shader* program)
{
	if (program->handle == -1)
		return;

	for (int i = 0; i < program->sources.size(); ++i)
	{
		render_shader_source* working_source = &program->sources[i];

		render_shader_source::destroy(working_source);
	}

	glDeleteProgram(program->handle);

	program->handle = -1;

	if (program->uniform_locations != nullptr)
	{
		delete program->uniform_locations;
	}
}

void render_shader::use(render_shader* program)
{
	if (program->handle == -1)
	{
		render_shader::compile(program);
	}

	glUseProgram(program->handle);
}

void render_shader::uniform_mat4(render_shader* program, std::string name, glm::mat4 data)
{
	use(program);

	int uniform_location = render_shader::get_uniform_location(program, name);
	glUniformMatrix4fv(uniform_location, 1, false, (float*)&data);
}

int render_shader::get_uniform_location(render_shader* program, std::string name)
{
	if (program->handle == -1)
		return -1;

	if (program->uniform_locations->find(name) != program->uniform_locations->end())
	{
		return (*program->uniform_locations)[name];
	}

	int new_uniform_location = glGetUniformLocation(program->handle, name.c_str());

	(*program->uniform_locations)[name] = new_uniform_location;

	return new_uniform_location;
}

void render_shader_source::compile(render_shader_source* source)
{
	render_shader_source::destroy(source);

	uint32_t new_handle = glCreateShader(source->shader_type);
	const char* c_string = source->source.c_str();
	int size_buffer = source->source.length();

	char error[1000];

	glShaderSource(new_handle, 1, &c_string, &size_buffer);
	glCompileShader(new_handle);

	glGetShaderInfoLog(new_handle, sizeof(error), &size_buffer, error);

	if (size_buffer != 0)
	{
		std::cerr << error << std::endl;

		assert(false); throw 0;
	}

	source->handle = new_handle;
}

void render_shader_source::destroy(render_shader_source* source)
{
	if (source->handle == -1)
		return;

	glDeleteShader(source->handle);

	source->handle = -1;
}
