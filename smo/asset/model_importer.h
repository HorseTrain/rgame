#pragma once

#ifndef MODEL_IMPORTER
#define MODEL_IMPORTER

#include <string>

struct r_asset_file;
struct render_model;
struct asset_collection;

void import_render_model_from_r_asset_file(render_model* result, r_asset_file* source, std::string name);
void import_render_model_from_r_asset_file(render_model** result, asset_collection* asset_collection_context, r_asset_file* source, std::string name);

#endif