#pragma once

#ifndef MARIO_H
#define MARIO_H

struct render_model;
struct asset_collection;
struct io;

struct actor_mario
{
	render_model* mario_body;
	render_model* mario_head;
	render_model* mario_hand_l;
	render_model* mario_hand_r;

	static void create(actor_mario* result, io* io_context,asset_collection* asset_collection_context);
	static void destroy(actor_mario* to_destroy);
};

#endif