#pragma once

#ifndef RENDER_SURFACE_H
#define RENDER_SURFACE_H

#include <string>
#include <inttypes.h>
#include <unordered_map>

struct render_shader;

enum render_surface_data_type
{
	float1,
	float2,
	float3,
	float4,
	int1,
	int2,
	int3,
	int4,
	mat4x4,
};

struct render_surface_data
{
	std::string					name;
	render_surface_data_type	data_type;
	uint8_t						data[1024];

	template <typename T>
	static T* get_data_ref(render_surface_data* render_surface_data_context)
	{
		return (T*)render_surface_data_context->data;
	}
};

struct render_surface
{
	std::unordered_map<std::string, render_surface_data>	surface_data;

	render_shader*											working_shader;
	bool													enable_depth;
	bool													enable_backface_culling;

	static void												create_surface_data(render_surface* render_surface_context, render_surface_data_type type, std::string name);
	static bool												surface_value_exists(render_surface* render_surface_context, std::string name);
	static void												upload_surface_data(render_surface* render_surface_context, render_surface_data* data);
	static void												create(render_surface* render_surface_result, render_shader* working_shader);
	static void												destroy(render_surface* render_surface_result);
	static void												use(render_surface* render_surface_context);
	static void												copy(render_surface* result, render_surface* source);

	template <typename T>
	static void												set_data(render_surface* render_surface_context, std::string name, T data, render_surface_data_type type)
	{
		create_surface_data(render_surface_context, type, name);

		*render_surface_data::get_data_ref<T>(&render_surface_context->surface_data[name]) = data;
	}
};

#endif