#include "obj_model_scene.h"
#include "rgame/game_scene.h"
#include "wavefront_mesh.h"
#include "rgame/render_window.h"
#include "rgame/render_shader.h"
#include "rgame/io.h"
#include "rgame/string_tools.h"
#include "rgame/wavefront.h"

const char* default_vertex = R"(#version 330 core

in vec3 i_position;
in vec3 i_normal;
in vec2 i_uv;
in vec4 i_color;

out vec3 position;
out vec3 normal;
out vec2 uv;
out vec4 color;

uniform mat4 object_transform;
uniform mat4 camera_transform;

vec3 transform_vec3(mat4 transformation, vec3 point)
{
	return (transformation * vec4(point, 1)).xyz;
}

void main()
{
	position = transform_vec3(object_transform, i_position);
	normal = transform_vec3(object_transform, i_normal);

	uv = i_uv;
	color = i_color;

	gl_Position = camera_transform * vec4(position, 1);
})";

const char* default_fragment = R"(#version 330 core

in vec3 position;
in vec3 normal;
in vec2 uv;
in vec4 color;

out vec4 frag_color;

void main()
{
	frag_color = vec4(normal, 1);
}

)";

struct wavefront_mesh_gameobject_data
{
	obj_model_scene*	scene;
	wavefront_mesh*		mesh;

	transform			object_transform;
	render_shader		shader;

	float				rotation_angle;
};

void wavefront_mesh_gameobject_update(game_object* object, game_scene* scene)
{
	wavefront_mesh_gameobject_data* data = (wavefront_mesh_gameobject_data*)&object->object_buffer;

	data->object_transform.position = { 0, 0, -20 };
	data->object_transform.rotation = glm::angleAxis(data->rotation_angle, glm::vec3(0, 1, 0));

	float delta_time = scene->window->delta_time;

	data->rotation_angle += delta_time * 0.01;

	render_shader* shader_in_use = &data->shader;

	render_shader::use(shader_in_use);

	int camera_transform_uniform = render_shader::get_uniform_location(shader_in_use, "camera_transform");
	int object_transform_uniform = render_shader::get_uniform_location(shader_in_use, "object_transform");

	glm::mat4 camera_matrix = render_camera::get_view_matrix(&scene->main_camera, render_window::get_aspect_ratio(scene->window));
	glm::mat4 object_transform = transform::get_local_transform_matrix(&data->object_transform);

	glUniformMatrix4fv(camera_transform_uniform, 1, false, (GLfloat*)&camera_matrix);
	glUniformMatrix4fv(object_transform_uniform, 1, false, (GLfloat*)&object_transform);

	glEnable(GL_DEPTH_TEST);

	render_mesh::draw(&data->mesh->mesh);
}

void update_scene_camera(obj_model_scene* scene)
{
	render_camera* camera = &scene->raw_scene->main_camera;

	camera->feild_of_view = 60;
	camera->z_near = 1;
	camera->z_far = 100;

	float delta_time = scene->raw_scene->window->delta_time;

	int x_input = glfwGetKey(scene->raw_scene->window->raw_window, GLFW_KEY_D) - glfwGetKey(scene->raw_scene->window->raw_window, GLFW_KEY_A);
	int y_input = glfwGetKey(scene->raw_scene->window->raw_window, GLFW_KEY_W) - glfwGetKey(scene->raw_scene->window->raw_window, GLFW_KEY_S);

	glm::mat4 camera_transform_matrix = transform::get_local_camera_transform_matrix(&camera->_transform);

	camera->_transform.position.z += (float)y_input * -delta_time * 0.8;
	camera->_transform.position.x += (float)x_input * delta_time * 0.8;
}

void obj_model_scene::create(obj_model_scene* result, game_scene* raw_scene, std::string obj_path)
{
	result->raw_scene = raw_scene;

	result->allocator = arena_allocator::create(10 * 1024 * 1024);
	result->meshes = intrusive_linked_list<wavefront_mesh*>::create(&result->allocator, nullptr, nullptr);

	load_obj_from_file(result, obj_path);

	raw_scene->main_camera._transform.position.y = 5;
}

void obj_model_scene::destroy(obj_model_scene* to_destroy)
{
	for (auto i = to_destroy->meshes->first; i != nullptr; i = i->next)
	{
		if (i->data == nullptr)
			continue;
		
		render_shader::destroy(&i->data->game_object_data->shader);

		delete i->data;
	}

	arena_allocator::destroy(&to_destroy->allocator);
}

void obj_model_scene::update(obj_model_scene* scene)
{
	update_scene_camera(scene);

	game_scene::scene_update(scene->raw_scene);
}

void obj_model_scene::create_empty_wavefront_mesh_object(obj_model_scene* scene, wavefront_mesh** result_ptr)
{
	wavefront_mesh* result = intrusive_linked_list<wavefront_mesh*>::insert_element(scene->meshes, new wavefront_mesh())->data;
	scene->raw_scene->objects.push_back(&result->_game_object);

	render_mesh::create(&result->mesh, GL_TRIANGLES);
	game_object::create_empty(&result->_game_object);

	result->_game_object.object_update = wavefront_mesh_gameobject_update;
	wavefront_mesh_gameobject_data* go_data = (wavefront_mesh_gameobject_data*)result->_game_object.object_buffer;

	go_data->rotation_angle = 0;

	go_data->mesh = result;
	go_data->scene = scene;

	render_shader::create(&go_data->shader);
	transform::create_identity(&go_data->object_transform);

	go_data->shader.sources.push_back(shader_source::create(GL_VERTEX_SHADER, default_vertex));
	go_data->shader.sources.push_back(shader_source::create(GL_FRAGMENT_SHADER, default_fragment));

	render_shader::compile(&go_data->shader);

	result->game_object_data = go_data;

	*result_ptr = result;
}

void obj_model_scene::load_obj_from_file(obj_model_scene* scene, std::string path)
{
	std::vector<std::string> lines;

	io_read_entire_file_lines(&lines, path);

	int working_line = 0;

	std::map<std::string, std::vector<glm::vec4>> vertex_data;

	while (true)
	{
		if (working_line >= lines.size())
			break;

		std::string working_string = lines[working_line];

		if (starts_with(working_string, "o "))
		{
			std::vector<std::string> parts;

			split_string(&parts, working_string, ' ');

			wavefront_mesh* working_mesh;

			create_empty_wavefront_mesh_object(scene, &working_mesh);

			working_mesh->name = parts[1];

			working_line = wavefront_parse_mesh_object(&working_mesh->mesh, &lines, &vertex_data,working_line + 1);
		}
		else
		{
			working_line++;
		}
	}
}
