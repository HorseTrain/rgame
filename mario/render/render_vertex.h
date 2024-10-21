#pragma once

#ifndef RENDER_VERTEX_H
#define RENDER_VERTEX_H

#include "rgame/glm/vec2.hpp"
#include "rgame/glm/vec3.hpp"
#include "rgame/glm/vec4.hpp"

struct render_vertex
{
	glm::vec2	uv_0;
	glm::vec2	uv_1;
	glm::vec2	uv_2;

	glm::vec3	position;
	glm::vec3	normal;
	glm::vec3	tangent;
	glm::vec3	bitangent;

	glm::vec4	color;
	glm::ivec4	weight_indices;
	glm::vec4	weights;
};

#endif