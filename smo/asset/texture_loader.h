#pragma once

#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>

struct io;
struct asset_collection;

void load_texture(std::string texture_name, std::string source_path, asset_collection* asset_collection_context, io* io_context);

#endif