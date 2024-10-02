#include "actor_mario.h"
#include "render/render_model.h"
#include "actor.h"
#include "level/level_scene.h"
#include "rgame/render_shader.h"
#include "game/game_context.h"
#include "asset/model_importer.h"
#include "rgame/render_window.h"

float angle = 0;

static void create_mario(actor* actor_context)
{
	actor_mario::create((actor_mario*)actor_context->actor_data);
}

static void update_mario(actor* actor_context)
{
	actor_mario::update((actor_mario*)actor_context->actor_data);
}

static void destroy_mario(actor* actor_context)
{
	actor_mario::destroy((actor_mario*)actor_context->actor_data);
}

static float find_average_distance(float* d0, float* d1, int count)
{
	float dis = 0;

	for (int i = 0; i < count; ++i)
	{
		dis += abs(d1[i] - d0[i]);
	}

	return dis / count;
}

static void mario_draw(actor_mario* mario)
{
	level_scene* working_scene = mario->level_scene_context;
	render_shader* working_shader = asset_collection::get_asset_by_name<render_shader>(&mario->game_context_context->game_assets, "unskinned_actor");

	render_shader::use(working_shader);

	transform* camera_transform = &mario->camera_transform;

	float aspect = render_window::get_aspect_ratio(working_scene->game_context_context->render_window_context);

	glm::mat4 camera_transform_matrix = transform::get_world_transform_matrix_recursive(camera_transform);

	camera_transform_matrix[3] = -camera_transform_matrix[3];
	camera_transform_matrix[3].w = -camera_transform_matrix[3].w;

	camera_transform_matrix = render_camera::get_raw_perspective(&working_scene->main_camera, aspect) * camera_transform_matrix;

	render_shader::uniform_mat4(working_shader, "camera_transform", camera_transform_matrix);
	render_shader::uniform_mat4(working_shader, "object_transform", transform::get_world_transform_matrix_recursive(&mario->actor_transform));

	glEnable(GL_DEPTH_TEST);

	render_model::draw(mario->mario_body);
}

static void update_camera(actor_mario* mario)
{
	input_manager* input = &mario->level_scene_context->game_context_context->input;

	float angle_x = input->mouse_x;
	float angle_y = input->mouse_y;

	angle += mario->game_context_context->render_window_context->delta_time * 0.1f;

	mario->actor_transform.position = glm::vec3(0, sin(angle) * 10, 0);

	mario->actor_transform.rotation = glm::angleAxis(angle, glm::vec3(0, 1, 0));


	mario->camera_transform.position = glm::vec3(0, 0, 200);
}

void actor_mario::create_actor(level_scene* level_scene_context)
{
	actor* actor_container = level_scene::create_empty_actor(level_scene_context);

	actor_container->actor_create = create_mario;
	actor_container->actor_update = update_mario;
	actor_container->actor_destroy = destroy_mario;

	actor_mario* mario = new actor_mario();

	mario->level_scene_context = level_scene_context;
	mario->game_context_context = level_scene_context->game_context_context;

	mario->actor_reference = actor_container;

	transform::create_identity(&mario->actor_transform);
	transform::create_identity(&mario->model_transform);
	transform::create_identity(&mario->camera_pivot);	
	transform::create_identity(&mario->camera_transform);

	transform::set_parent(&mario->actor_transform, &mario->model_transform);
	
	transform::set_parent(&mario->actor_transform, &mario->camera_transform);

	actor_container->actor_data = mario;
}

void actor_mario::create(actor_mario* to_create)
{
	//to_create->actor_transform.scale = glm::vec3(0.1f);
	to_create->camera_distance_to_player = 10;

	io* io_context = &to_create->game_context_context->io_context;
	asset_collection* asset_collection_context = &to_create->level_scene_context->asset_collection_context;

	import_render_model_from_path(&to_create->mario_body, asset_collection_context, "actor/mario/model.rasset", io_context, "mario_model");
}

void actor_mario::update(actor_mario* to_update)
{
	//controller
	
	//physics

	//camera
	update_camera(to_update);

	//draw
	mario_draw(to_update);
}

void actor_mario::destroy(actor_mario* to_destroy)
{
	delete to_destroy;
}
