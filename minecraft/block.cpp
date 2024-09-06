#include "block.h"

bool block::get_is_transparent(block* block_context)
{
    return block_context->block_id == 0;
}
