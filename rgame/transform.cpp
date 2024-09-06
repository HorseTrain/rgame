#include "transform.h"

void transform::create_identity(transform* result)
{
	result->children = std::vector<transform*>();
	result->parent_transform = nullptr;

	result->position = glm::vec3(0, 0, 0);
	result->rotation = glm::quat(1, 0, 0, 0);
	result->scale = glm::vec3(1);
}

glm::mat4 transform::get_local_transform_matrix(transform* source)
{
	glm::mat4 position_matrix = glm::translate(glm::mat4(1), source->position);
	glm::mat4 rotation_matrix = glm::mat4_cast(source->rotation);
	glm::mat4 scale_matrix = glm::scale(glm::mat4(1), source->scale);

	return position_matrix * rotation_matrix * scale_matrix;
}

glm::mat4 transform::get_local_camera_transform_matrix(transform* source)
{
	glm::mat4 position_matrix = glm::translate(glm::mat4(1), -source->position);
	glm::mat4 rotation_matrix = glm::mat4_cast(source->rotation);
	glm::mat4 scale_matrix = glm::scale(glm::mat4(1), source->scale);

	return position_matrix * rotation_matrix * scale_matrix;
}
