#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include <inttypes.h>

struct block
{
	uint16_t		block_id;
	static bool		get_is_transparent(block* block_context);
};

#endif