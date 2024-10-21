#ifndef MEMORY_TOOLS_H
#define MEMORY_TOOLS_H

#include <inttypes.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>

#include <iostream>
#include <vector>

enum allocator_type
{
	non_free,
	free_allowed
};

struct arena_allocator
{
public:

	uint64_t			buffer;
	uint64_t			buffer_size;

	uint64_t			buffer_top;
	arena_allocator*	child;

	allocator_type		type;

	uint64_t			heap_bottom;
	uint64_t			heap_top;

	struct heap_allocation
	{
		void* buffer_location;
		uint64_t allocation_size;
	};

	static arena_allocator create(uint64_t size, allocator_type type = non_free)
	{
		arena_allocator result;

		result.buffer = (uint64_t)malloc(size + sizeof(arena_allocator));
		result.buffer_size = size;

		result.child = nullptr;
		result.buffer_top = 0;

		result.type = type;

		result.heap_bottom = 0;
		result.heap_top = size;

		return result;
	}

	static void destroy(arena_allocator* allocator)
	{
		if (allocator->child != nullptr)
		{
			destroy(allocator->child);
		}

		free((void*)allocator->buffer);
	}
	
	static void* allocate_recursive(arena_allocator* allocator, uint64_t size)
	{
		switch (allocator->type)
		{
		case non_free:
		{
			uint64_t result = allocator->buffer_top;
			uint64_t new_location = result + size;

			if (new_location >= allocator->buffer_size)
			{
				if (allocator->child == nullptr)
				{
					create_child(allocator, size);
				}

				return allocate_recursive(allocator->child, size);
			}

			allocator->buffer_top = new_location;

			return (void*)(allocator->buffer + result);
		}; break;

		case free_allowed:
		{
			assert(false); throw 0;

			for (uint64_t heap_bottom_location = allocator->buffer; heap_bottom_location < allocator->buffer + allocator->heap_bottom; heap_bottom_location += sizeof(heap_allocation))
			{
				heap_allocation* working_heap_allocation = (heap_allocation*)heap_bottom_location;


			}
		}; break;

		default: assert(false); throw 0;
		}
	}

	template <typename T>
	static T* allocate_struct(arena_allocator* allocator, uint64_t count = 1)
	{
		if (count == 0)
			return nullptr;

		return (T*)allocate_recursive(allocator, count * sizeof(T));
	}

private:
	static void create_child(arena_allocator* allocator, uint64_t new_size)
	{
		assert(allocator->child == nullptr);

		allocator->child = (arena_allocator*)(allocator->buffer + allocator->buffer_size);

		if (allocator->buffer_size > new_size)
		{
			new_size = allocator->buffer_size;
		}

		*allocator->child = create(new_size + 1024);
	}
};

template <typename T>
struct intrusive_linked_list_element
{
	T								data;

	intrusive_linked_list_element*	next;
	intrusive_linked_list_element*	prev;

	void*							parent_list;
};

template <typename T>
struct intrusive_linked_list
{
	arena_allocator*					allocator;

	intrusive_linked_list_element<T>*	first;
	intrusive_linked_list_element<T>*	last;

	static intrusive_linked_list_element<T>* create_empty_element(intrusive_linked_list<T>* list)
	{
		intrusive_linked_list_element<T>* result = arena_allocator::allocate_struct<intrusive_linked_list_element<T>>(list->allocator);

		result->prev = nullptr;
		result->next = nullptr;

		result->parent_list = list;

		return result;
	}

	static intrusive_linked_list_element<T>* create_element(intrusive_linked_list<T>* list, T data)
	{
		intrusive_linked_list_element<T>* result = create_empty_element(list);

		result->data = data;

		return result;
	}

	static intrusive_linked_list<T>* create(arena_allocator* allocator,T first, T last)
	{
		intrusive_linked_list<T>* result = arena_allocator::allocate_struct<intrusive_linked_list<T>>(allocator);

		result->allocator = allocator;

		result->first = create_element(result, first);
		result->last = create_element(result, last);

		result->first->next = result->last;
		result->last->prev = result->first;

		return result;
	}

	static bool is_first_element(intrusive_linked_list<T>* list, intrusive_linked_list_element<T>* element)
	{
		return list->first == element;
	}

	static bool is_last_element(intrusive_linked_list<T>* list, intrusive_linked_list_element<T>* element)
	{
		return list->last == element;
	}

	static void insert_element_after(intrusive_linked_list<T>* list, intrusive_linked_list_element<T>* to_insert_after, intrusive_linked_list_element<T>* element)
	{
		assert(to_insert_after->parent_list = list);
		assert(element->parent_list == list);

		assert(!is_last_element(list, to_insert_after));

		auto last_p_0 = to_insert_after;
		auto last_p_1 = to_insert_after->next;

		last_p_0->next = element;
		last_p_1->prev = element;

		element->next = last_p_1;
		element->prev = last_p_0;
	}

	static void insert_element_before(intrusive_linked_list<T>* list, intrusive_linked_list_element<T>* working, intrusive_linked_list_element<T>* element)
	{
		assert(working->parent_list = list);
		assert(element->parent_list == list);

		assert(!is_first_element(list, working));

		insert_element_after(list, working->prev, element);
	}

	static void insert_element(intrusive_linked_list<T>* list, intrusive_linked_list_element<T>* element)
	{
		insert_element_before(list, list->last, element);
	}

	static intrusive_linked_list_element<T>* insert_element(intrusive_linked_list<T>* list, T value)
	{
		intrusive_linked_list_element<T>* raw_element = create_element(list, value);

		insert_element(list, raw_element);

		return raw_element;
	}
};

template <typename T>
struct fast_array
{
	uint64_t	count;
	T*			data;

	T& operator [] (uint64_t index)
	{
		assert(index < count);

		return data[index];
	}

	static void create(arena_allocator* allocator, int count, fast_array<T>* result)
	{
		result->count = count;
		result->data = arena_allocator::allocate_struct<T>(allocator, count);
	}

	static void create(fast_array<T>* result, int count)
	{
		result->count = count;
		result->data = new T[count];
	}

	static void copy_from(fast_array* array, T* data)
	{
		//i hate this.
		for (int i = 0; i < array->count; ++i)
		{
			array->data[i] = data[i];
		}
	}
};

#endif