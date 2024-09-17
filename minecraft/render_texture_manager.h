#pragma once

#ifndef RENDER_TEXTURE_MANAGER_H
#define RENDER_TEXTURE_MANAGER_H

#include <string>

struct render_texture;
struct graphics_asset_manager;
struct io;

void create_render_texture(render_texture** result, graphics_asset_manager* manager, std::string name);
void load_texture_from_path(render_texture** result, graphics_asset_manager* manager, std::string name, io* io_context, std::string path);

#endif