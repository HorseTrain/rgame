#pragma once

#ifndef MEMORY_TOOLS_H
#define MEMORY_TOOLS_H

#include <string>

template <typename T>
struct array_container
{
	T*	data;
	int count;

	static void create(array_container* result, int count)
	{
		result->data = (T*)malloc(count * sizeof(T));
		result->count = count;
	}

	static void destroy(array_container* array_context)
	{
		free(array_context->data);
	}
};

#endif