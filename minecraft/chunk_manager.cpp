#include "chunk_manager.h"
#include "chunk.h"
#include "player.h"
#include "world.h"
#include "chunk_mesh.h"
#include "game.h"
#include "graphics_asset_manager.h"
#include "rgame/render_shader.h"
#include "rgame/render_window.h"
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

			if (glm::distance(glm::vec3(new_position), player_position) > render_distance)
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

	render_shader::use(chunk_shader);

	glm::mat4 camera_transform = render_camera::get_view_matrix(working_camera_context, render_window::get_aspect_ratio(render_window_context));
	glUniformMatrix4fv(render_shader::get_uniform_location(chunk_shader, "camera_transform"), 1, false, (float*)&camera_transform);

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

		glUniform3f(render_shader::get_uniform_location(chunk_shader, "chunk_offset"), working_chunk->position.x, working_chunk->position.y, working_chunk->position.z);

		chunk_mesh::draw(working_chunk_mesh);
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

	std::unordered_map<KEY_TYPE, chunk*> all_chunks = chunk_manager_context->chunks.chunks;

	for (auto i = all_chunks.begin(); i != all_chunks.end(); ++i)
	{
		chunk* working_chunk = i->second;

		if (working_chunk->marked_for_destruction)
		{
			chunk_store::ghost_chunk(chunk_store_context, working_chunk);
		}
	}

	chunk_store::destroy_ghost_chunks(chunk_store_context);

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
}

static void update_current_chunks(chunk_manager* chunk_manager_context)
{
	chunk_store* chunk_store_context = &chunk_manager_context->chunks;
	std::unordered_map<KEY_TYPE, chunk*> chunks = chunk_manager_context->chunks.chunks;

	std::vector<chunk*> to_render;
	std::vector<chunk*> to_generate_neighbors;

	chunk_store_context->ghost_chunks_lock.lock();

	auto destroyed_chunks_last = chunk_store_context->main_thread_destroyed_chunks;
	chunk_store_context->main_thread_destroyed_chunks = std::unordered_set<chunk*>();

	chunk_store_context->ghost_chunks_lock.unlock();

	for (auto i = chunks.begin(); i != chunks.end(); ++i)
	{
		chunk* working_chunk = i->second;

		if (destroyed_chunks_last.find(working_chunk) != destroyed_chunks_last.end())
			continue;

		if (working_chunk->marked_for_destruction)
		{
			continue;
		}

		if (!chunk::in_render_distance(working_chunk) && working_chunk->data_ready)
		{
			working_chunk->marked_for_destruction = true;

			continue;
		}

		if (!working_chunk->data_ready || !working_chunk->mesh_ready)
		{
			chunk_store::append_chunk_to_creation_que(chunk_store_context, working_chunk);
		}
		else
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

	chunk_store::create(&chunk_manager_context->chunks,chunk_manager_context);

	thread_manager::create(&chunk_manager_context->data_creation_thread, create_chunk_data);
	thread_manager::create(&chunk_manager_context->mesh_creation_thread, create_chunk_mesh_data);

	thread_manager::start_thread(&chunk_manager_context->data_creation_thread, { chunk_manager_context });
	thread_manager::start_thread(&chunk_manager_context->mesh_creation_thread, { chunk_manager_context });

	chunk_manager_context->render_distance = 20;
}

void chunk_manager::destroy(chunk_manager* chunk_manager_context)
{
	chunk_store::destroy(&chunk_manager_context->chunks);

	thread_manager::destroy(&chunk_manager_context->data_creation_thread);
	thread_manager::destroy(&chunk_manager_context->mesh_creation_thread);
}

void chunk_manager::update(chunk_manager* chunk_manager_context)
{
	uint64_t arguments[] = { (uint64_t)chunk_manager_context };

	update_current_chunks(chunk_manager_context);

	//create_chunk_data(arguments, 1);
	//create_chunk_mesh_data(arguments, 1);
}
