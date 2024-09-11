#include <iostream>

#include "rgame/render_window.h"
#include "rgame/render_shader.h"
#include "graphics_asset_manager.h"
#include "render_shader_manager.h"
#include "render_texture_manager.h"
#include "rgame/render_texture.h"
#include "render_texture_manager.h"
#include "rgame/render_texture.h"
#include "chunk_mesh.h"
#include "player.h"

graphics_asset_manager gfx;

void loop(render_window* window, void* arguments)
{
	graphics_asset_manager::create(&gfx);

	render_shader* test_shader;
	render_texture* test_texture;
	player test_player;

	chunk_mesh working_mesh;
	chunk_mesh::create(&working_mesh);

	create_render_shader(&test_shader, &gfx, "default_shader");

	render_shader::append_shader_source(test_shader, GL_VERTEX_SHADER, R"(#version 330 core

in int vertex_data_0;

out vec3 position;
out vec3 normal;

void get_data()
{
	position = vec3(vertex_data_0 & 31, (vertex_data_0 >> 5) & 31, (vertex_data_0 >> 15) & 31);

	int normal_index = (vertex_data_0 >> 15) & 7;

	switch (normal_index)
	{
		case 0: normal = vec3(-1, 0, 0); break;
		case 1: normal = vec3(1, 0, 0); break;
		case 2: normal = vec3(0, -1, 0); break;
		case 3: normal = vec3(0, 1, 0); break;
		case 4: normal = vec3(0, 0, -1); break;
		case 5: normal = vec3(0, 0, 1); break;
	}
}

void main()
{	
	get_data();

	gl_Position = vec4(position, 1);
}
)");

	render_shader::append_shader_source(test_shader, GL_FRAGMENT_SHADER, R"(#version 330 core

out vec4 frag_color;

in vec3 position;
in vec3 normal;

uniform sampler2D tex;

void main()
{
	frag_color = texture(tex, vec2(position.x, 1-position.y));
}
)");

	render_shader::compile(test_shader);
	render_shader::use(test_shader);

	create_render_texture(&test_texture, &gfx, "default_texture");

	render_texture::load_from_file(test_texture, "C:\\Users\\raymo\\OneDrive\\Desktop\\.antlr\\1200x1200bb.jpg");
	render_texture::use(test_texture, 0);

	working_mesh.vertex_data.push_back(chunk_vertex::create(0, 0, 0, 1, 0, 0));
	working_mesh.vertex_data.push_back(chunk_vertex::create(1, 0, 0, 1, 0, 0));
	working_mesh.vertex_data.push_back(chunk_vertex::create(1, 1, 0, 1, 0, 0));
	working_mesh.vertex_data.push_back(chunk_vertex::create(0, 1, 0, 1, 0, 0));

	chunk_mesh::upload(&working_mesh);

	chunk_mesh::draw(&working_mesh);

	graphics_asset_manager::destroy(&gfx);
}

int main()
{
	render_window test_window;

	render_window::create(&test_window, loop, 500, 500, 60, "minecraft");
	render_window::run(&test_window, nullptr);

	render_window::destroy(&test_window);
}