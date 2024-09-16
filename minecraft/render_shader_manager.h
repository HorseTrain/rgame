#pragma once

#ifndef RENDER_SHADER_MANAGER
#define RENDER_SHADER_MANAGER

#include <string>

struct render_shader;
struct graphics_asset_manager;
struct io;

void create_render_shader(render_shader** result, graphics_asset_manager* manager, std::string name);
void load_shader_from_path(render_shader** resultm, graphics_asset_manager* manager, std::string name, io* io_context, std::string path);

#endif