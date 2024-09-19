#pragma once

#ifndef RENDER_VERTEX
#define RENDER_VERTEX

#include <rgame/glm/vec2.hpp>
#include <rgame/glm/vec3.hpp>
#include <rgame/glm/vec4.hpp>

struct render_vertex
{
	glm::vec2	uv_0;
	glm::vec2	uv_1;

	glm::vec3	position;
	glm::vec3	normal;
	glm::vec3	tangent;
	glm::vec3	bi_tangnet;

	glm::ivec4	weight_index;
	glm::vec4	weight_data;
};

#endif