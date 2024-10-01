#pragma once

#ifndef UNIFORM_DATA_H
#define UNIFORM_DATA_H

#include <string>

enum uniform_data_type
{
	type_integer,
	type_float,
	type_double,
	type_mat4,
	type_vec3,
	type_vec2,
};

struct uniform_data
{
	std::string			name;
	uniform_data_type	type;
	uint8_t				buffer[64];

	template <typename T>
	T& get_reference(uniform_data* uniform_data_context)
	{
		return *(T*)uniform_data_context->buffer;
	}
};

#endif