#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "rgame/render_window.h"

#include "rgame/io.h"
#include "rgame/r_asset.h"
#include "asset/model_importer.h"
#include "render/render_model.h"
#include "rgame/render_shader.h"
#include "rgame/render_camera.h"
#include "asset/asset_instance.h"

const char* vertex = R"(

#version 330 core

layout (location = 0) in vec2		i_uv_0;
layout (location = 1) in vec2		i_uv_1;
layout (location = 2) in vec3		i_position;
layout (location = 3) in vec3		i_normal;
layout (location = 4) in vec3		i_tangent;
layout (location = 5) in vec3		i_bi_tangnet;
layout (location = 6) in ivec4		i_weight_index;
layout (location = 7) in vec4		i_weight_data;
layout (location = 8) in vec4		i_color;

out vec2	uv_0;
out vec2	uv_1;
out vec3	position;
out vec3	normal;
out vec3	tangent;
out vec3	bi_tangnet;
out vec4	color;

uniform mat4 camera_transform;
uniform mat4 object_transform;

void main()
{
	position = i_position;
	normal = i_normal;
	color = i_color;

	uv_0= i_uv_0;

	gl_Position = camera_transform * object_transform * vec4(position, 1);
}

)";

const char* fragment = R"(

#version 330 core

in vec2			uv_0;
in vec2			uv_1;
in vec3			position;
in vec3			normal;
in vec3			tangent;
in vec3			bi_tangnet;
in vec4			color;

out vec4 frag_color;

void main()
{
	frag_color = vec4(uv_0, 0, 1);
}

)";

render_model* model;
render_shader shader;
render_camera camera;

bool shader_loaded = false;

float angle = 0;

static void run_game(render_window* window, void* arguments)
{
	//

	if (!shader_loaded)
	{
		render_shader::create(&shader);
		render_shader::append_shader_source(&shader, GL_VERTEX_SHADER, vertex);
		render_shader::append_shader_source(&shader, GL_FRAGMENT_SHADER, fragment);

		render_shader::compile(&shader);
		render_shader::use(&shader);

		shader_loaded = true;
	}

	float spect = render_window::get_aspect_ratio(window);

	glm::mat4 camera_transform = render_camera::get_view_matrix(&camera, spect);
	glm::mat4 object_transform = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0));

	render_camera::get_view_matrix(&camera, spect);

	camera.transform_context.position = glm::vec3(0, 50, 200);

	angle += window->delta_time * 0.03f;

	glUniformMatrix4fv(render_shader::get_uniform_location(&shader, "camera_transform"), 1, false, (float*)&camera_transform);
	glUniformMatrix4fv(render_shader::get_uniform_location(&shader, "object_transform"), 1, false, (float*)&object_transform);

	glEnable(GL_DEPTH_TEST);

	render_model::draw(model);
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	render_window working_window;

	io io_context;
	io::create(&io_context, "D:\\smod\\model\\");

	r_asset_file model_file;
	r_asset_file::create_from_path(&io_context, &model_file, "model.rasset");

	asset_collection c;

	import_render_model_from_r_asset_file(&model, &c, &model_file, "cube nigga");

	render_camera::create_empty(&camera, 60, 1, 1000);

	r_asset_file::destroy(&model_file);

	render_window::create(&working_window, run_game, 500, 500, 60, "MARIO");

	render_window::run(&working_window, nullptr);

	render_shader::destroy(&shader);

	asset_collection::destroy(&c);

	render_window::destroy(&working_window);

	return -1;
}