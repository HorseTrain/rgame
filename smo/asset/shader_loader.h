#pragma once

#include <string>

#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

struct asset_collection;
struct io;
struct render_shader;

void load_shader(std::string shader_name,std::string source_path, asset_collection* asset_collection_context, io* io_context);

#endif