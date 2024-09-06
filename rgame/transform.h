#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/mat4x4.hpp"

struct transform;

struct transform
{
	glm::vec3				position;
	glm::quat				rotation;
	glm::vec3				scale;

	transform*				parent_transform;
	std::vector<transform*> children;

	static void				create_identity(transform* result);
	static glm::mat4		get_local_transform_matrix(transform* source);
	static glm::mat4		get_local_camera_transform_matrix(transform* source);
	static glm::mat4		get_world_transform_matrix_recursive(transform* source);
};

#endif