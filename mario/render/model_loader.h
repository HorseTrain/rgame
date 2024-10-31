#pragma once

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "rgame/r_asset.h"
#include "rgame/lifetime_memory_manager.h"
#include <inttypes.h>
#include "level.h"

struct r_asset_file;
struct level;
struct render_surface;

enum primative_type : int32_t
{
	triangle,
	quad,
	line,
	points,
};

template <typename T>
static void load_render_mesh(T** result_object,static_render_mesh** result_mesh,r_asset_file* r_asset_file_context, level* level_context, int location)
{
	r_asset_file::seek(r_asset_file_context, location);

	std::string mesh_name = r_asset_file::read_string(r_asset_file_context);
	primative_type mesh_primative_type = r_asset_file::read< primative_type>(r_asset_file_context);
	int has_weights = r_asset_file::read<int>(r_asset_file_context);

	uint64_t vertex_buffer = (uint64_t)r_asset_file::get_sub_buffer(r_asset_file_context);
	uint64_t index_buffer = (uint64_t)r_asset_file::get_sub_buffer(r_asset_file_context);

	int vertex_count = *(int*)vertex_buffer;
	render_vertex* verticies = (render_vertex*)(vertex_buffer + 4);

	int index_count = *(int*)index_buffer;
	uint16_t* indicies = (uint16_t*)(index_buffer + 4);

	static_render_mesh* working_mesh = lifetime_memory_manager::allocate< static_render_mesh>(&level_context->memory);

	static_render_mesh::create(working_mesh, index_count, vertex_count, GL_TRIANGLES);

	fast_array<uint16_t>::copy_from(&working_mesh->indecies, indicies);
	fast_array<render_vertex>::copy_from(&working_mesh->vertices, verticies);

	*result_mesh = working_mesh;
	T::create(result_object, level_context);
}

/*
template <typename T>
static void load_rmodel(T** game_object_context, level* level_context, r_asset_file* r_asset_file_context, render_surface* working_surface)
{
	T::create(game_object_context, level_context);

	int mesh_count = r_asset_file::read<int>(r_asset_file_context);

	for (int i = 0; i < mesh_count; ++i)
	{
		int mesh_location = r_asset_file::read_pointer(r_asset_file_context);
		int return_location = r_asset_file::peek(r_asset_file_context);

		static_render_mesh* result_mesh;
		T* child_object;

		load_render_mesh<T>(&child_object,&result_mesh,r_asset_file_context,level_context, mesh_location);

		mesh_renderer::create<static_render_mesh>(&child_object->mesh_renderer_context, working_surface, result_mesh);

		r_asset_file::seek(r_asset_file_context, return_location);

		transform::set_parent(&(*game_object_context)->object_transform, &child_object->object_transform);
	}

	mesh_renderer::create<static_render_mesh>(&(*game_object_context)->mesh_renderer_context, nullptr, nullptr);
}
*/

#endif