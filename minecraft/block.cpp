#include "block.h"
#include "chunk.h"

glm::ivec3 block::get_global_position(block* block_context)
{
	return glm::vec3(block_context->local_position) + glm::vec3(block_context->chunk_context->position);
}
