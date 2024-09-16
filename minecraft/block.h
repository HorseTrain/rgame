#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include <inttypes.h>
#include "rgame/r_math.h"
#include "rgame/glm/vec3.hpp"

struct chunk;

struct block
{
	chunk*	chunk_context;
	uint8_t	block_id;
	bool	is_transparent;
	ivec3b	local_position;

	static glm::ivec3 get_global_position(block* block_context);
};

#endif