#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <string>
#include <map>

#include "glm/mat4x4.hpp"

struct render_shader;

struct render_shader_source
{
	uint32_t					handle;

	uint32_t					shader_type;
	std::string					source;

	static void					compile(render_shader_source* source);
	static void					destroy(render_shader_source* source);
	static render_shader_source	create(uint32_t type, std::string source);
};

struct render_shader
{
	uint32_t							handle;
	std::vector<render_shader_source>	sources;
	std::map<std::string, int>*			uniform_locations;

	static void					create(render_shader* result);
	static void					append_shader_source(render_shader* program, uint32_t type, std::string source_code);
	static void					compile(render_shader* program);
	static void					destroy(render_shader* program);
	static void					use(render_shader* program);
	static void					uniform_mat4(render_shader* program,std::string name, glm::mat4 data);
	static void					uniform_vec3(render_shader* program, std::string name, glm::vec3 data);
	static void					uniform_vec2(render_shader* program, std::string name, glm::vec2 data);
	static int					get_uniform_location(render_shader* program, std::string name);
} ;


#endif