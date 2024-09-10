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
	for (auto i = chunk_store::get_start(&to_destroy->chunks); i != chunk_store::get_end(&to_destroy->chunks); ++i)
	{
		chunk::destroy_allocation(i->second);
	}
}

std::string chunk_manager::get_chunk_key(int x, int y, int z)
{
	return std::to_string(x) + std::to_string(y) + std::to_string(z);
}

void chunk_manager::request_chunk_for_creation(chunk_manager* chunk_manager_context, int x, int y, int z)
{
	float render_distance = chunk_manager_context->world_context->render_distance * CHUNK_SIZE;

	if (glm::distance(glm::vec3(x, y, z), glm::vec3(chunk_manager_context->world_context->main_player.transform_context.position)) > render_distance)
		return;

	bool exists = chunk_store::chunk_exists(&chunk_manager_context->chunks, x, y, z);

	if (!exists)
	{
		chunk* new_chunk = chunk::allocate_chunk();

		chunk::create(new_chunk, x, y, z, chunk_manager_context);

		chunk_manager_context->chunks.chunk_creation_que.push_back(new_chunk);

		chunk_store::set_chunk(&chunk_manager_context->chunks, x, y, z, new_chunk);
	}
}

void chunk_manager::generate_chunk_data_in_que(chunk_manager* chunk_manager_context)
{
	std::vector<chunk*> to_create = std::vector<chunk*>(chunk_manager_context->chunks.chunk_creation_que);
	chunk_manager_context->chunks.chunk_creation_que = std::vector<chunk*>();

	auto start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < to_create.size(); ++i)
	{
		chunk* working_chunk = to_create[i];
			 
		if (!working_chunk->data_generated)
		{
			chunk::generate_chunk_data(working_chunk);

			continue;
		}

		auto elapsed_miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();

#if RELEASE
		if (elapsed_miliseconds > 10)
			return;
#endif

		if (!working_chunk->mesh_generated)
		{
			chunk::generate_chunk_mesh(working_chunk);

			continue;
		}
	}
}

void chunk_manager::destroy_chunk_data_in_que(chunk_manager* chunk_manager_context)
{
	std::vector<chunk*> to_destroy = std::vector<chunk*>(chunk_manager_context->chunks.chunk_destruction_que);
	chunk_manager_context->chunks.chunk_destruction_que = std::vector<chunk*>();

	for (int i = 0; i < to_destroy.size(); ++i)
	{
		chunk* working_chunk = to_destroy[i];

		chunk_store::remove_chunk(&chunk_manager_context->chunks, working_chunk);
		chunk::destroy(working_chunk);		
	}
}

static void draw_chunks(chunk_manager* chunk_manager_context,std::vector<chunk*>* to_draw)
{
	game_scene* working_scene = chunk_manager_context->world_context->raw_scene;
	world_graphics_manager* world_graphics_manager_context = &chunk_manager_context->world_context->world_graphics_manager_context;

	glm::mat4 camera_transform = render_camera::get_view_matrix(&working_scene->main_camera, render_window::get_aspect_ratio(working_scene->window));

	render_shader* working_shader = world_graphics_manager_context->loaded_shaders["default"];

	render_shader::use(working_shader);
	glUniformMatrix4fv(render_shader::get_uniform_location(working_shader, "camera_transform"), 1, false, (float*) & camera_transform);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < to_draw->size(); ++i)
	{
		chunk* working_chunk = to_draw->data()[i];

		if (working_chunk->chunk_mesh.vertices.size() == 0)
			continue;

		glm::mat4 object_transform = glm::translate(glm::mat4(1), glm::vec3(working_chunk->x, working_chunk->y, working_chunk->z));

		glUniformMatrix4fv(render_shader::get_uniform_location(working_shader, "object_transform"), 1, false, (float*)&object_transform);

		if (!working_chunk->chunk_mesh.uploaded)
		{
			render_mesh::upload(&working_chunk->chunk_mesh);
		}

		render_mesh::draw(&working_chunk->chunk_mesh);
	}	
}

void chunk_manager::update_existing_chunks(chunk_manager* chunk_manager_context)
{
	std::vector<chunk*> full_chunks;
	std::vector<chunk*> chunks_that_can_generate;
	std::vector<chunk*> to_destroy;

	glm::vec3 player_forward = transform::get_forward(transform::get_local_camera_transform_matrix(&chunk_manager_context->world_context->main_player.transform_context));
	glm::vec3 player_point = chunk_manager_context->world_context->main_player.transform_context.position;

	for (auto i = chunk_store::get_start(&chunk_manager_context->chunks); i != chunk_store::get_end(&chunk_manager_context->chunks); ++i)
	{
		chunk* working_chunk = i->second;

		if (!chunk::in_render_distance(working_chunk))
		{
			to_destroy.push_back(working_chunk);

			continue;
		}

		glm::vec3 chunk_location = chunk::get_position(working_chunk);
		glm::vec3 direction = chunk_location - player_point;

		float dot_angle = 0;

		if (glm::length(direction) > CHUNK_SIZE)
		{
			direction = glm::normalize(direction);

			dot_angle = glm::dot(direction, player_forward);
		}

		if (!working_chunk->data_generated || !working_chunk->mesh_generated)
		{
			insert_chunk_into_data_generation_que(chunk_manager_context, working_chunk);

			chunks_that_can_generate.push_back(working_chunk);

			continue;
		}

		full_chunks.push_back(working_chunk);
	}

	if (full_chunks.size() == 0)
		return;

	for (int i = 0; i < chunks_that_can_generate.size(); ++i)
	{
		chunk_manager::generate_neighbor_chunks(chunk_manager_context, chunks_that_can_generate[i]);
	}

	draw_chunks(chunk_manager_context,&full_chunks);

	for (int i = 0; i < to_destroy.size(); ++i)
	{
		chunk* working_chunk = to_destroy[i];

		chunk_store::remove_chunk(&chunk_manager_context->chunks, working_chunk);

		chunk::destroy_allocation(working_chunk);
	}	
}

void chunk_manager::chunk_manager_update(chunk_manager* chunk_manager_context)
{
	update_existing_chunks(chunk_manager_context);

	generate_chunk_data_in_que(chunk_manager_context);
	destroy_chunk_data_in_que(chunk_manager_context);
}

void chunk_manager::insert_chunk_into_data_generation_que(chunk_manager* chunk_manager_context, chunk* chunk_context)
{
	chunk_manager_context->chunks.chunk_creation_que.push_back(chunk_context);
}

void chunk_manager::insert_chunk_into_data_destruction_que(chunk_manager* chunk_manager_context, chunk* chunk_context)
{
	chunk_manager_context->chunks.chunk_destruction_que.push_back(chunk_context);
}

chunk* chunk_manager::get_chunk(chunk_manager* chunk_manager_context, int x, int y, int z)
{
	std::string key = chunk_manager::get_chunk_key(x, y, z);

	chunk* result;

	if (!chunk_store::chunk_exists(&chunk_manager_context->chunks, x, y, z))
	{
		result = nullptr;
	}
	else
	{
		result = chunk_store::get_chunk(&chunk_manager_context->chunks, x, y, z);
	}	

	return result;
}

void chunk_manager::generate_neighbor_chunks(chunk_manager* chunk_manager_context, chunk* chunk_context)
{
	int x = chunk_context->x;
	int y = chunk_context->y;
	int z = chunk_context->z;

	//if (chunk_context->is_secure)
	//	return;

	request_chunk_for_creation(chunk_manager_context, x + CHUNK_SIZE,	y + 0,				z + 0);
	request_chunk_for_creation(chunk_manager_context, x + -CHUNK_SIZE,	y + 0,				z + 0);
	request_chunk_for_creation(chunk_manager_context, x + 0,			y + CHUNK_SIZE,		z + 0);
	request_chunk_for_creation(chunk_manager_context, x + 0,			y + -CHUNK_SIZE,	z + 0);
	request_chunk_for_creation(chunk_manager_context, x + 0,			y + 0,				z + CHUNK_SIZE);
	request_chunk_for_creation(chunk_manager_context, x + 0,			y + 0,				z + -CHUNK_SIZE);
}