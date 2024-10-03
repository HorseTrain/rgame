#include "spinning_mario_actor.h"
#include "level/level_scene.h"
#include "asset/model_importer.h"
#include "asset/asset_collection.h"
#include "game/game_context.h"
#include "rgame/render_shader.h"
#include "rgame/render_window.h"
#include "rgame/render_texture.h"
#include "actor.h"

static void create_mario(actor* level_actor)
{
	spinning_mario_actor* mario = (spinning_mario_actor*)level_actor->actor_data;

	mario->model = asset_collection::get_asset_by_name<render_model>(&level_actor->level_scene_context->asset_collection_context, "mario");

	if (mario->model == nullptr)
	{
		import_render_model_from_path(&mario->model, &level_actor->level_scene_context->asset_collection_context, "model.rasset", &level_actor->game_context_context->io_context, "mario");
	}

	mario->_transform.scale = glm::vec3(0.03f);
}

static void update_mario(actor* level_actor)
{
	spinning_mario_actor* mario = (spinning_mario_actor*)level_actor->actor_data;

	render_camera* working_camera = &level_actor->level_scene_context->main_camera;
	render_shader* working_shader = asset_collection::get_asset_by_name<render_shader>(&level_actor->game_context_context->game_assets, "unskinned_actor");

	render_window* working_window = level_actor->level_scene_context->game_context_context->render_window_context;

	mario->mario_angle += working_window->delta_time * 0.01f;
	mario->_transform.rotation = glm::angleAxis(mario->mario_angle, glm::vec3(0, 1, 0));

	render_shader::use(working_shader);
	render_shader::uniform_mat4(working_shader, "camera_transform", render_camera::get_view_matrix(working_camera, render_window::get_aspect_ratio(working_window)));
	render_shader::uniform_mat4(working_shader, "object_transform", transform::get_world_transform_matrix_recursive(&mario->_transform));

	render_texture* working_texture = asset_collection::get_asset_by_name<render_texture>(&level_actor->level_scene_context->asset_collection_context, "mario_body");
	render_texture::use(working_texture,0);

	glEnable(GL_DEPTH_TEST);

	render_model::draw(mario->model);
}

static void destroy_mario(actor* level_actor)
{
	spinning_mario_actor* mario = (spinning_mario_actor*)level_actor->actor_data;

	delete mario;
}

spinning_mario_actor* spinning_mario_actor::create(level_scene* scene)
{
	spinning_mario_actor* result = new spinning_mario_actor();

	actor* level_actor = level_scene::create_empty_actor(scene);
	level_actor->actor_data = result;

	transform::create_identity(&result->_transform);

	level_actor->actor_create = create_mario;
	level_actor->actor_update = update_mario;
	level_actor->actor_destroy = destroy_mario;

	return result;
}
