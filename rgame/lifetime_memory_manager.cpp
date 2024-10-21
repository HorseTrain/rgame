#include "lifetime_memory_manager.h"
#include <iostream>

void lifetime_memory_manager::create(lifetime_memory_manager* result)
{
	//
}

void lifetime_memory_manager::destroy(lifetime_memory_manager* lifetime_memory_manager_context)
{
	for (auto i : lifetime_memory_manager_context->raw_allocations)
	{
		lifetime_memory_manager::delete_allocation(i, false);
	}
}

bool lifetime_memory_manager::allocation_exists(lifetime_memory_manager* lifetime_memory_manager_context, std::string name)
{
	return lifetime_memory_manager_context->allocation_name_map.find(name) != lifetime_memory_manager_context->allocation_name_map.end();
}

void lifetime_memory_manager::delete_allocation(memory_allocation* allocation_context, bool erase_from_raw_collections)
{
	if (allocation_context->name != "")
	{
		allocation_context->lifetime_memory_manager_context->allocation_name_map.erase(allocation_context->name);
	}

	allocation_context->buffer_destruction(allocation_context->buffer);
	allocation_context->buffer_deletion(allocation_context->buffer);

	allocation_context->lifetime_memory_manager_context->allocation_map.erase(allocation_context->buffer);

	if (erase_from_raw_collections)
	{
		allocation_context->lifetime_memory_manager_context->raw_allocations.erase(allocation_context);
	}

	delete allocation_context;
}

void lifetime_memory_manager::delete_buffer(lifetime_memory_manager* lifetime_memory_manager_context, void* buffer)
{
	if (lifetime_memory_manager_context->allocation_map.find(buffer) == lifetime_memory_manager_context->allocation_map.end())
	{
		assert(false);

		throw 0;
	}

	delete_allocation(lifetime_memory_manager_context->allocation_map[buffer]);
}