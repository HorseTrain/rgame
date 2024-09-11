#include "chunk_manager.h"

void chunk_manager::create(chunk_manager* chunk_manager_context, world* world_context)
{
	chunk_manager_context->world_context = world_context;
}

void chunk_manager::destroy(chunk_manager* chunk_manager_context)
{
	//TODO
}
