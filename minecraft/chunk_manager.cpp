#include "chunk_manager.h"
#include "chunk.h"
#include "rgame/render_shader.h"
#include "rgame/render_window.h"
#include "rgame/glm/mat4x4.hpp"
#include "rgame/render_camera.h"
#include "rgame/game_scene.h"
#include "rgame/render_mesh.h"
#include "world.h"

void chunk_manager::create(chunk_manager* result, world* world_context)
{
	result->world_context = world_context;

	request_chunk_for_creation(result, 0, 0, 0);

	request_chunk_for_creation(result, CHUNK_SIZE, 0, 0);
	request_chunk_for_creation(result, -CHUNK_SIZE, 0, 0);
	request_chunk_for_creation(result, 0, CHUNK_SIZE, 0);
	request_chunk_for_creation(result, 0, -CHUNK_SIZE, 0);
	request_chunk_for_creation(result, 0, 0, CHUNK_SIZE);
	request_chunk_for_creation(result, 0, 0, -CHUNK_SIZE);
}

void chunk_manager::destroy(chunk_manager* to_destroy)
{
	for (auto i = to_destroy->chunks.begin(); i != to_destroy->chunks.end(); ++i)
	{
		chunk::destroy_allocation(i->second);
	}
}

std::string chunk_manager::get_chunk_key(int x, int y, int z)
{
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

void chunk_manager::request_chunk_for_creation(chunk_manager* chunk_manager_context, int x, int y, int z)
{
	std::string chunk_key = get_chunk_key(x, y, z);

	chunk_manager_context->chunk_lock.lock();

	bool exists = chunk_manager_context->chunks.find(chunk_key) != chunk_manager_context->chunks.end();

	if (!exists)
	{
		chunk* new_chunk = chunk::allocate_chunk();

		chunk::create(new_chunk, x, y, z, chunk_manager_context);

		chunk_manager_context->chunk_creation_que.push_back(new_chunk);

		chunk_manager_context->chunks[chunk_key] = new_chunk;
	}

	chunk_manager_context->chunk_lock.unlock();
}

void chunk_manager::generate_chunk_data_in_que(chunk_manager* chunk_manager_context)
{
	chunk_manager_context->chunk_lock.lock();

	std::vector<chunk*> to_create = std::vector<chunk*>(chunk_manager_context->chunk_creation_que);
	chunk_manager_context->chunk_creation_que = std::vector<chunk*>();

	chunk_manager_context->chunk_lock.unlock();

	for (int i = 0; i < to_create.size(); ++i)
	{
		chunk* working_chunk = to_create[i];

		if (!working_chunk->data_generated)
		{
			chunk::generate_chunk_data(working_chunk);

			continue;
		}

		if (!working_chunk->mesh_generated)
		{
			chunk::generate_chunk_mesh(working_chunk);

			continue;
		}
	}
}

static float angle = 0;

static void draw_chunks(chunk_manager* chunk_manager_context,std::vector<chunk*>* to_draw)
{
	game_scene* working_scene = chunk_manager_context->world_context->raw_scene;
	world_graphics_manager* world_graphics_manager_context = &chunk_manager_context->world_context->world_graphics_manager_context;

	glm::mat4 camera_transform = render_camera::get_view_matrix(&working_scene->main_camera, render_window::get_aspect_ratio(working_scene->window));
	glm::mat4 object_transform = glm::mat4(1);

	render_shader* working_shader = world_graphics_manager_context->loaded_shaders["default"];

	render_shader::use(working_shader);
	glUniformMatrix4fv(render_shader::get_uniform_location(working_shader, "object_transform"), 1, false, (float*) & object_transform);
	glUniformMatrix4fv(render_shader::get_uniform_location(working_shader, "camera_transform"), 1, false, (float*) & camera_transform);

	//angle += working_scene->window->delta_time * 0.01;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < to_draw->size(); ++i)
	{
		chunk* working_chunk = to_draw->data()[i];

		if (!working_chunk->chunk_mesh.uploaded)
		{
			render_mesh::upload(&working_chunk->chunk_mesh);
		}

		render_mesh::draw(&working_chunk->chunk_mesh);
	}	
}

void chunk_manager::update_existing_chunks(chunk_manager* chunk_manager_context)
{
	std::vector<chunk*> to_draw;

	for (auto i = chunk_manager_context->chunks.begin(); i != chunk_manager_context->chunks.end(); ++i)
	{
		chunk* working_chunk = i->second;

		if (!working_chunk->data_generated || !working_chunk->mesh_generated)
		{
			insert_chunk_into_data_generation_que(chunk_manager_context, working_chunk);

			continue;
		}

		to_draw.push_back(working_chunk);
	}

	if (to_draw.size() == 0)
		return;

	draw_chunks(chunk_manager_context,&to_draw);
}

void chunk_manager::chunk_manager_update(chunk_manager* chunk_manager_context)
{
	generate_chunk_data_in_que(chunk_manager_context);

	update_existing_chunks(chunk_manager_context);
}

void chunk_manager::insert_chunk_into_data_generation_que(chunk_manager* chunk_manager_context, chunk* chunk_context)
{
	chunk_manager_context->chunk_lock.lock();

	chunk_manager_context->chunk_creation_que.push_back(chunk_context);

	chunk_manager_context->chunk_lock.unlock();
}

chunk* chunk_manager::get_chunk(chunk_manager* chunk_manager_context, int x, int y, int z)
{
	std::string key = chunk_manager::get_chunk_key(x, y, z);

	chunk* result;

	chunk_manager_context->chunk_lock.lock();

	if (chunk_manager_context->chunks.find(key) == chunk_manager_context->chunks.end())
	{
		result = nullptr;
	}
	else
	{
		result = chunk_manager_context->chunks[key];
	}

	chunk_manager_context->chunk_lock.unlock();

	return result;
}
