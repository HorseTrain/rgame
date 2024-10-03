#include "floor_actor.h"
#include "actor.h"
#include "level/level_scene.h"
#include "rgame/render_window.h"
#include "rgame/render_shader.h"
#include "game/game_context.h"
#include "rgame/render_texture.h"
#include "actor_ids.h"

static void create_floor(actor* level_actor)
{
	floor_actor* floor = (floor_actor*)level_actor->actor_data;

	static_render_mesh::create(&floor->floor_mesh, GL_QUADS, 4, 4);

	float ws = floor->size / 2;

	floor->floor_mesh.vertices[0].position = glm::vec3(ws, 0, ws);
	floor->floor_mesh.vertices[1].position = glm::vec3(-ws, 0, ws);
	floor->floor_mesh.vertices[2].position = glm::vec3(-ws, 0, -ws);
	floor->floor_mesh.vertices[3].position = glm::vec3(ws, 0, -ws);

	floor->floor_mesh.vertices[0].uv_0 = glm::vec2(ws, ws);
	floor->floor_mesh.vertices[1].uv_0 = glm::vec2(-ws, ws);
	floor->floor_mesh.vertices[2].uv_0 = glm::vec2(-ws, -ws);
	floor->floor_mesh.vertices[3].uv_0 = glm::vec2(ws, -ws);

	for (int i = 0; i < 4; ++i)
	{
		floor->floor_mesh.indices[i] = i;
	}
}

static void update_floor(actor* level_actor)
{
	floor_actor* floor = (floor_actor*)level_actor->actor_data;

	render_camera* working_camera = &level_actor->level_scene_context->main_camera;
	render_shader* working_shader = asset_collection::get_asset_by_name<render_shader>(&level_actor->game_context_context->game_assets,"unskinned_actor");

	render_window* working_window = level_actor->level_scene_context->game_context_context->render_window_context;

	render_shader::use(working_shader);
	render_shader::uniform_mat4(working_shader, "camera_transform", render_camera::get_view_matrix(working_camera, render_window::get_aspect_ratio(working_window)));
	render_shader::uniform_mat4(working_shader, "object_transform", transform::get_local_transform_matrix(&floor->floor_transform));

	if (floor->move)
	{
		floor->time += working_window->delta_time * 0.005f;
		floor->velocity = glm::vec3(0, 0, sin(floor->time) * 0.05f) * working_window->delta_time;

		floor->floor_transform.position += floor->velocity;

		render_texture* working_texture = asset_collection::get_asset_by_name<render_texture>(&level_actor->level_scene_context->asset_collection_context, "troll_face");
		render_texture::use(working_texture, 0);
	}
	else
	{
		render_texture* working_texture = asset_collection::get_asset_by_name<render_texture>(&level_actor->level_scene_context->asset_collection_context, "troll_face_1");
		render_texture::use(working_texture, 0);
	}

	glEnable(GL_DEPTH_TEST);

	static_render_mesh::draw(&floor->floor_mesh);
}

static void destroy_floor(actor* level_actor)
{
	floor_actor* floor = (floor_actor*)level_actor->actor_data;

	static_render_mesh::destroy(&floor->floor_mesh);

	delete floor;
}

floor_actor* floor_actor::create(level_scene* scene, float size)
{
	floor_actor* floor_actor_data = new floor_actor();

	actor* level_actor = level_scene::create_empty_actor(scene);
	level_actor->actor_data = floor_actor_data;

	level_actor->actor_id = actor_id::actor_id_floor;

	floor_actor_data->size = size;

	transform::create_identity(&floor_actor_data->floor_transform);

	floor_actor_data->actor_context = level_actor;

	level_actor->actor_create = create_floor;
	level_actor->actor_update = update_floor;
	level_actor->actor_destroy = destroy_floor;

	floor_actor_data->move = false;

	return floor_actor_data;
}
