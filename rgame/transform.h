#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <unordered_set>

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/mat4x4.hpp"

struct transform;

struct transform
{
	glm::vec3				position;
	glm::quat				rotation;
	glm::vec3				scale;

	transform*						parent_transform;
	std::unordered_set<transform*>	children;

	static  void set_parent(transform*new_parent, transform*child_transform);

	static void				create_identity(transform* result);
	static glm::mat4		get_local_transform_matrix(transform* source);
	static glm::mat4		get_local_camera_transform_matrix(transform* source);
	static glm::mat4		get_world_transform_matrix_recursive(transform* source);
	static void				set_from_world_transform(transform* transform_context,const glm::mat4 transform);

	static glm::vec3		get_forward(glm::mat4 source);
	static glm::vec3		get_right(glm::mat4 source);
	static glm::vec3		get_up(glm::mat4 source);
};

#endif