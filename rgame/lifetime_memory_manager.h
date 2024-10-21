#ifndef LIFETIME_MEMORY_MANAGER_H
#define LIFETIME_MEMORY_MANAGER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>

struct lifetime_memory_manager;
struct memory_allocation;

typedef void(*destruction_function_ptr)(void* allocation);

struct memory_allocation
{
	void*							buffer;
	destruction_function_ptr		buffer_destruction;
	destruction_function_ptr		buffer_deletion;
	std::string						name;
	lifetime_memory_manager*		lifetime_memory_manager_context;
};

struct lifetime_memory_manager
{
	std::unordered_set<memory_allocation*>				raw_allocations;
	std::unordered_map<std::string, memory_allocation*>	allocation_name_map;
	std::unordered_map<void*, memory_allocation*>		allocation_map;

	static void											create(lifetime_memory_manager* result);
	static void											destroy(lifetime_memory_manager* lifetime_memory_manager_context);
	static bool											allocation_exists(lifetime_memory_manager* lifetime_memory_manager_context, std::string name);
	static void											delete_allocation(memory_allocation* allocation_context, bool erase_from_raw_collections = true);
	static void											delete_buffer(lifetime_memory_manager* lifetime_memory_manager_context, void* buffer);

	template <typename T>
	static void delete_raw_allocation(void* to_delete)
	{
		delete (T*)to_delete;
	}

	template <typename T>
	static T* get_allocation(lifetime_memory_manager* lifetime_memory_manager_context, std::string name)
	{
		if (!allocation_exists(lifetime_memory_manager_context, name))
		{
			assert(false);

			throw 0;
		}

		return (T*)(lifetime_memory_manager_context->allocation_name_map[name]->buffer);
	}

	template <typename T>
	static T* allocate(lifetime_memory_manager* lifetime_memory_manager_context, std::string name = "")
	{
		T*					raw_data = new T();
		memory_allocation*	allocation = new memory_allocation();

		allocation->buffer = raw_data;
		allocation->name = name;
		allocation->lifetime_memory_manager_context = lifetime_memory_manager_context;

		allocation->buffer_destruction = (destruction_function_ptr)T::destroy;
		allocation->buffer_deletion = lifetime_memory_manager::delete_raw_allocation<T>;

		lifetime_memory_manager_context->raw_allocations.insert(allocation);
		lifetime_memory_manager_context->allocation_map[raw_data] = allocation;

		if (name != "")
		{
			if (lifetime_memory_manager::allocation_exists(lifetime_memory_manager_context, name))
			{
				assert(false);

				throw 0;
			}

			lifetime_memory_manager_context->allocation_name_map[name] = allocation;
		}

		return raw_data;
	}
};

#endif