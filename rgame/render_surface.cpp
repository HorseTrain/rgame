#include "render_surface.h"
#include "render_shader.h"
#include "render_window.h"
#include "rgame/render_texture.h"

void render_surface::create_surface_data(render_surface* render_surface_context, render_surface_data_type type, std::string name)
{
	if (surface_value_exists(render_surface_context, name))
	{
		if (render_surface_context->surface_data[name].data_type != type)
		{
			assert(false);

			throw 0;
		}

		return;
	}

	render_surface_context->surface_data[name].name = name;
	render_surface_context->surface_data[name].data_type = type;
}

bool render_surface::surface_value_exists(render_surface* render_surface_context, std::string name)
{
	return render_surface_context->surface_data.find(name) != render_surface_context->surface_data.end();
}

void render_surface::upload_surface_data(render_surface* render_surface_context, render_surface_data* data)
{
	render_shader::use(render_surface_context->working_shader);

	int uniform_location = render_shader::get_uniform_location(render_surface_context->working_shader, data->name);

	if (uniform_location == -1)
		return;

	switch (data->data_type)
	{
		case float1: { glUniform1f(uniform_location, *render_surface_data::get_data_ref<float>(data)); }break;
		case float2: { glm::vec2 to_upload = *render_surface_data::get_data_ref<glm::vec2>(data); glUniform2f(uniform_location, to_upload.x, to_upload.y); } break;
		case float3: { glm::vec3 to_upload = *render_surface_data::get_data_ref<glm::vec3>(data); glUniform3f(uniform_location, to_upload.x, to_upload.y, to_upload.z); }break;
		case float4: { glm::vec4 to_upload = *render_surface_data::get_data_ref<glm::vec4>(data); glUniform4f(uniform_location, to_upload.x, to_upload.y, to_upload.z, to_upload.w); } break;

		case int1: { glUniform1i(uniform_location, *render_surface_data::get_data_ref<int>(data)); }break;
		case int2: { glm::ivec2 to_upload = *render_surface_data::get_data_ref<glm::vec2>(data); glUniform2i(uniform_location, to_upload.x, to_upload.y); } break;
		case int3: { glm::ivec3 to_upload = *render_surface_data::get_data_ref<glm::vec3>(data); glUniform3i(uniform_location, to_upload.x, to_upload.y, to_upload.z); }break;
		case int4: { glm::ivec4 to_upload = *render_surface_data::get_data_ref<glm::vec4>(data); glUniform4i(uniform_location, to_upload.x, to_upload.y, to_upload.z, to_upload.w); } break;

		case mat4x4: { glm::mat4 to_upload = *render_surface_data::get_data_ref<glm::mat4>(data); glUniformMatrix4fv(uniform_location, 1, false, (float*)&to_upload); }; break;
	
		default: assert(false); throw 0;
	}
}

void render_surface::create(render_surface* render_surface_result, render_shader* working_shader)
{
	render_surface_result->working_shader = working_shader;
	render_surface_result->enable_depth = true;
	render_surface_result->enable_backface_culling = true;

	memset(render_surface_result->textures, 0, sizeof(render_surface::textures));
}

void render_surface::destroy(render_surface* render_surface_result)
{
	//TODO
}

void render_surface::use(render_surface* render_surface_context)
{
	for (auto i : render_surface_context->surface_data)
	{
		render_surface_data working_render_surface_data = i.second;

		upload_surface_data(render_surface_context, &working_render_surface_data);
	}

	render_shader::use(render_surface_context->working_shader);

	for (int i = 0; i < sizeof(render_surface::textures) / sizeof(render_texture*); ++i)
	{
		if (render_surface_context->textures[i] == nullptr)
			continue;

		render_texture::use(render_surface_context->textures[i], i);
	}

	if (render_surface_context->enable_depth)
	{
		glEnable(GL_DEPTH_TEST);
	}

	if (render_surface_context->enable_backface_culling)
	{
		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
}

void render_surface::copy(render_surface* result, render_surface* source)
{
	result->working_shader = source->working_shader;

	result->surface_data = source->surface_data;

	result->enable_depth = source->enable_depth;
	result->enable_backface_culling = source->enable_backface_culling;
}
