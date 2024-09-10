#include "block.h"
#include "assert.h"

bool block::get_is_transparent(block* block_context)
{
    if (block_context == nullptr)
    {
        assert(false);
    }

    return block_context->block_id == 0;
}
