#pragma once

#ifndef ASSET_HELPER_H
#define ASSET_HELPER_H

struct render_shader;
struct render_surface;
struct lifetime_memory_manager;

void create_render_shader_copy(render_shader** result, render_shader* source, lifetime_memory_manager* memory);
void create_render_surface_copy(render_surface** result, render_surface* source, lifetime_memory_manager* memory, bool copy_shader = true);

#endif