#define GLM_ENABLE_EXPERIMENTAL

#include "transform.h"
#include "glm/gtx/matrix_decompose.hpp"

void transform::create_identity(transform* result)
{
	result->children = std::unordered_set<transform*>();
	result->parent_transform = nullptr;

	result->position = glm::vec3(0, 0, 0);
	result->rotation = glm::quat(1, 0, 0, 0);
	result->scale = glm::vec3(1);
}

void transform::set_parent(transform* new_parent, transform* child_transform)
{
	if (child_transform->parent_transform != nullptr)
	{
		child_transform->parent_transform->children.erase(child_transform);
	}

	new_parent->children.insert(child_transform);

	child_transform->parent_transform = new_parent;
}

glm::mat4 transform::get_local_transform_matrix(transform* source)
{
	glm::mat4 position_matrix = glm::translate(glm::mat4(1), source->position);
	glm::mat4 rotation_matrix = glm::mat4_cast(source->rotation);
	glm::mat4 scale_matrix = glm::scale(glm::mat4(1), source->scale);

	return scale_matrix * rotation_matrix * position_matrix;
}

glm::mat4 transform::get_local_camera_transform_matrix(transform* source)
{
	glm::mat4 position_matrix = glm::translate(glm::mat4(1), -source->position);
	glm::mat4 rotation_matrix = glm::mat4_cast(source->rotation);

	return rotation_matrix * position_matrix;
}

glm::mat4 transform::get_world_transform_matrix_recursive(transform* source)
{
	if (source->parent_transform == nullptr)
	{
		return get_local_transform_matrix(source);
	}

	return get_local_transform_matrix(source) * get_world_transform_matrix_recursive(source->parent_transform);
}

void transform::set_from_world_transform(transform* transform_context, const glm::mat4 transform)
{
	//NOTES

	//for some reason, extracting quaternion data just is not working
	//so this will be stubbed for now gang.

	throw 0;
}

glm::vec3 transform::get_forward(glm::mat4 source)
{
	return glm::vec3(source[0][2], source[1][2], source[2][2]);
}

glm::vec3 transform::get_right(glm::mat4 source)
{
	return glm::vec3(source[0][0], source[1][0], source[2][0]);
}

glm::vec3 transform::get_up(glm::mat4 source)
{
	return glm::vec3(source[0][1], source[1][1], source[2][1]);
}