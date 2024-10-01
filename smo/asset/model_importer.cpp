#include "model_importer.h"
#include "render/static_render_mesh.h"
#include "rgame/r_asset.h"
#include "asset/r_asset_primative_type.h"
#include <assert.h>
#include "rgame/render_window.h"
#include "render/render_model.h"

template <typename T>
void read_buffer(array_container<T>* result_location, r_asset_file* source)
{
	char* data = (char*)r_asset_file::get_sub_buffer(source);
	int count = *(int*)data;

	void* raw_data = (void*)(data + 4);
	array_container<T>::create(result_location, count);

	memcpy(result_location->data, raw_data,count * sizeof(T));
}

void import_render_mesh_from_buffer(render_model* result, r_asset_file* source, int offset)
{
	r_asset_file::seek(source, offset);

	static_render_mesh* result_mesh;
	int gl_primative_type;
	bool has_weights;

	std::string mesh_name = r_asset_file::read_string(source);

	render_model::create_static_render_mesh(result, &result_mesh, mesh_name);

	r_asset_primative_type primative_type = r_asset_file::read< r_asset_primative_type>(source);

	switch (primative_type)
	{
	case r_asset_primative_type::triangle: gl_primative_type = GL_TRIANGLES; break;
	default: assert(false); throw 0;
	}

	has_weights = r_asset_file::read<int>(source);

	read_buffer<render_vertex>(&result_mesh->vertices, source);
	read_buffer<uint16_t>(&result_mesh->indices, source);

	static_render_mesh::create(result_mesh, gl_primative_type, 0, 0);

	result->meshes[mesh_name] = result_mesh;
}

void import_render_model_from_r_asset_file(render_model* result, r_asset_file* source, std::string name)
{
	r_asset_file::seek(source, 0);

	render_model::create(result, name);

	int mesh_count = r_asset_file::read<int>(source);

	for (int i = 0; i < mesh_count; ++i)
	{
		int mesh_data_offset = r_asset_file::read_pointer(source);
		int return_location = r_asset_file::peek(source);

		import_render_mesh_from_buffer(result, source, mesh_data_offset);

		r_asset_file::seek(source, return_location);
	}
}

void import_render_model_from_r_asset_file(render_model** result, asset_collection* asset_collection_context, r_asset_file* source, std::string name)
{
	render_model* result_model;
	render_model::create_with_asset_collection(asset_collection_context, &result_model, name);

	import_render_model_from_r_asset_file(result_model, source, name);

	*result = result_model;
}
