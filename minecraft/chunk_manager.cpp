#include "chunk_manager.h"
#include "chunk.h"
#include "player.h"
#include "world.h"
#include "chunk_mesh.h"
#include "game.h"
#include "graphics_asset_manager.h"
#include "rgame/render_shader.h"
#include "rgame/render_window.h"
#include "rgame/render_texture.h"
#include <iostream>

static void generate_chunks_around_player(chunk_manager* chunk_manager_context)
{
	player* main_player = &chunk_manager_context->world_context->main_player;

	glm::vec3 player_position = main_player->transform_context.position;
	player_position /= (float)CUBE_CHUNK_SIZE;
	glm::ivec3 working_i_position = glm::ivec3(player_position) * CUBE_CHUNK_SIZE;

	chunk* entry_chunk = chunk_store::create_or_get_chunk(&chunk_manager_context->chunks, working_i_position);

	for (int i = 0; i < 6; ++i)
	{
		chunk_store::create_or_get_chunk(&chunk_manager_context->chunks, chunk::request_chunk_neighbor_offset(entry_chunk, i));
	}
}

static void generate_chunks_around_que(chunk_manager* chunk_manager_context, std::vector<chunk*>* sources)
{
	player* main_player = &chunk_manager_context->world_context->main_player;
	glm::vec3 player_position = main_player->transform_context.position;

	float render_distance = chunk_manager_context->render_distance * CUBE_CHUNK_SIZE;

	for (int s = 0; s < sources->size(); ++s)
	{
		chunk* entry_chunk = (*sources)[s];

		for (int i = 0; i < 6; ++i)
		{
			glm::ivec3 new_position = chunk::request_chunk_neighbor_offset(entry_chunk, i);

			if (glm::distance(glm::vec3(new_position), player_position) >= render_distance)
				continue;

			chunk_store::create_or_get_chunk(&chunk_manager_context->chunks, chunk::request_chunk_neighbor_offset(entry_chunk, i));
		}
	}
}

static void render_chunks(chunk_manager* chunk_manager_context, std::vector<chunk*>* to_render)
{
	world* world_context = chunk_manager_context->world_context;
	game* game_context = world_context->game_context;
	graphics_asset_manager* graphics_asset_manager_context = &game_context->graphics_asset_manager_context;
	render_camera* working_camera_context = &world_context->main_camera;
	render_window* render_window_context = game_context->window_context;

	render_shader* chunk_shader = (render_shader*)graphics_asset_manager_context->assets["default_chunk_shader"]->data;
	render_texture* chunk_texture = (render_texture*)graphics_asset_manager_context->assets["default_chunk_texture"]->data;

	render_shader::use(chunk_shader);
	render_texture::use(chunk_texture, 0);

	glm::mat4 camera_transform = render_camera::get_view_matrix(working_camera_context, render_window::get_aspect_ratio(render_window_context));
	glUniformMatrix4fv(render_shader::get_uniform_location(chunk_shader, "camera_transform"), 1, false, (float*)&camera_transform);
	glUniform2f(render_shader::get_uniform_location(chunk_shader, "texture_size"), chunk_texture->width, chunk_texture->height);
	glUniform1i(render_shader::get_uniform_location(chunk_shader, "face_texture_size"), 16);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	for (int i = 0; i < to_render->size(); ++i)
	{
		chunk* working_chunk = (*to_render)[i];
		chunk_mesh* working_chunk_mesh = working_chunk->chunk_mesh_context;

		if (!working_chunk_mesh->uploaded)
		{
			chunk_mesh::upload(working_chunk_mesh);
		}

		glUniform1f(render_shader::get_uniform_location(chunk_shader, "highlight"), working_chunk->highlight);
		glUniform3f(render_shader::get_uniform_location(chunk_shader, "chunk_offset"), working_chunk->position.x, working_chunk->position.y, working_chunk->position.z);

		chunk_mesh::draw(working_chunk_mesh);

		working_chunk->highlight -= render_window_context->delta_time * 0.05f;

		if (working_chunk->highlight < 0)
		{
			working_chunk->highlight = 0;
		}
	}
}

static void create_chunk_data(uint64_t* arguments, int argument_count)
{
	chunk_manager* chunk_manager_context = (chunk_manager*)arguments[0];
	chunk_store* chunk_store_context = &chunk_manager_context->chunks;

	if (chunk_store::creation_que_is_empty(chunk_store_context))
		return;

	std::vector<chunk*> to_create = chunk_store::get_vector_from_creation_que(chunk_store_context);

	for (int i = 0; i < to_create.size(); ++i)
	{
		chunk* working_chunk = to_create[i];

		if (working_chunk->marked_for_destruction)
			continue;

		if (working_chunk->data_ready)
			continue;

		chunk::generate_data(working_chunk);

		working_chunk->data_ready = true;
	}
}

static void create_chunk_mesh_data(uint64_t* arguments, int argument_count)
{
	chunk_manager* chunk_manager_context = (chunk_manager*)arguments[0];
	chunk_store* chunk_store_context = &chunk_manager_context->chunks;

	if (chunk_store::creation_que_is_empty(chunk_store_context))
		return;

	std::vector<chunk*> to_create = chunk_store::get_vector_from_creation_que(chunk_store_context);

	for (int i = 0; i < to_create.size(); ++i)
	{
		chunk* working_chunk = to_create[i];

		if (working_chunk->marked_for_destruction)
		{
			continue;
		}

		if (working_chunk->data_ready && !working_chunk->mesh_ready && chunk::ready_for_mesh_generation(working_chunk))
		{
			chunk::generate_mesh(working_chunk);

			working_chunk->mesh_ready = true;
		}
	}

	std::unordered_set<chunk*>* ghost_que = &chunk_manager_context->chunks.ghosted_chunks;
	std::vector<chunk*> to_destroy;

	for (auto i = ghost_que->begin(); i != ghost_que->end(); ++i)
	{
		uint64_t times[] = { chunk_manager_context->time, chunk_manager_context->data_creation_thread.time, chunk_manager_context->mesh_creation_thread.time };

		uint64_t min_time = chunk::get_minimum_destruction_time(*i, times);

		if (min_time > 2)
		{
			to_destroy.push_back(*i);
		}
	}

	auto start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < to_destroy.size(); ++i)
	{
		auto end_time = std::chrono::high_resolution_clock::now();

		auto elapsed_miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

		if (elapsed_miliseconds > 1)
			return;

		chunk_store_context->ghost_chunks_lock.lock();
		ghost_que->erase(to_destroy[i]);
		chunk_store_context->ghost_chunks_lock.unlock();

		chunk::destroy(to_destroy[i]);
	}
}

static bool chunk_in_player_thrustum(chunk* chunk_context, player* player_context)
{
	glm::vec3 player_position = player_context->transform_context.position;
	glm::vec3 chunk_position = chunk_context->position;

	glm::vec3 direction = glm::normalize(chunk_position - player_position);

	float angle = abs(glm::dot(direction, player_context->forward));

	if (glm::length(player_position - (glm::vec3)chunk_context->position) < CUBE_CHUNK_SIZE * 5)
	{
		return true;
	}

	return angle > 0.5f;
}

static void update_current_chunks(chunk_manager* chunk_manager_context)
{
	chunk_store* chunk_store_context = &chunk_manager_context->chunks;
	std::unordered_map<KEY_TYPE, chunk*> chunks = chunk_manager_context->chunks.chunks;

	std::vector<chunk*> to_render;
	std::vector<chunk*> to_generate_neighbors;
	
	for (auto i = chunks.begin(); i != chunks.end(); ++i)
	{
		chunk* working_chunk = i->second;

		if (working_chunk == nullptr)
			continue;

		if (working_chunk->marked_for_destruction)
		{
			continue;
		}

		if (!chunk::in_render_distance(working_chunk) && working_chunk->data_ready && !working_chunk->generating_data)
		{
			chunk_manager::ready_chunk_for_destruction(chunk_manager_context, working_chunk);

			continue;
		}

		if (!chunk_in_player_thrustum(working_chunk, &chunk_manager_context->world_context->main_player))
		{
			continue;
		}

		if (!working_chunk->data_ready || !working_chunk->mesh_ready)
		{
			chunk_store::append_chunk_to_creation_que(chunk_store_context, working_chunk);
		}
		else if (working_chunk->non_transparent != 0)
		{
			to_render.push_back(working_chunk);
		}

		if (chunk::ready_for_neighbor_generation(working_chunk))
		{
			to_generate_neighbors.push_back(working_chunk);
		}
	}

	render_chunks(chunk_manager_context, &to_render);
	generate_chunks_around_que(chunk_manager_context, &to_generate_neighbors);
	generate_chunks_around_player(chunk_manager_context);
}

void chunk_manager::create(chunk_manager* chunk_manager_context, world* world_context)
{
	chunk_manager_context->world_context = world_context;

	perlin::create(&chunk_manager_context->normal_noise, 0);
	perlin::create(&chunk_manager_context->mountain_noise, 0);
	perlin::create(&chunk_manager_context->mini_noise, 0);

	chunk_store::create(&chunk_manager_context->chunks,chunk_manager_context);

	thread_manager::create(&chunk_manager_context->data_creation_thread, create_chunk_data);
	thread_manager::create(&chunk_manager_context->mesh_creation_thread, create_chunk_mesh_data);

	thread_manager::start_thread(&chunk_manager_context->data_creation_thread, { chunk_manager_context });
	thread_manager::start_thread(&chunk_manager_context->mesh_creation_thread, { chunk_manager_context });

	chunk_manager_context->time = 0;

	chunk_manager_context->render_distance = RENDER_DISTANCE;
}

void chunk_manager::destroy(chunk_manager* chunk_manager_context)
{
	thread_manager::destroy(&chunk_manager_context->data_creation_thread);
	thread_manager::destroy(&chunk_manager_context->mesh_creation_thread);

	chunk_store::destroy(&chunk_manager_context->chunks);
}

void chunk_manager::update(chunk_manager* chunk_manager_context)
{
	uint64_t arguments[] = { (uint64_t)chunk_manager_context };

	chunk_manager_context->time++;

	update_current_chunks(chunk_manager_context);
}

void chunk_manager::ready_chunk_for_destruction(chunk_manager* chunk_manager_context, chunk* working_chunk)
{
	working_chunk->marked_for_destruction = true;

	uint64_t times[] = { chunk_manager_context->time, chunk_manager_context->data_creation_thread.time, chunk_manager_context->mesh_creation_thread.time };

	chunk_store::ghost_chunk(&chunk_manager_context->chunks, working_chunk, times);
}
