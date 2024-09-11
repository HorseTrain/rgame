#pragma once

#ifndef RENDER_SHADER_MANAGER
#define RENDER_SHADER_MANAGER

#include <string>

struct render_shader;
struct graphics_asset_manager;

void create_render_shader(render_shader** result, graphics_asset_manager* manager, std::string name);

#endif