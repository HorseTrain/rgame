#pragma once

#ifndef RENDER_MATERIAL_H
#define RENDER_MATERIAL_H

#include "rgame/render_texture.h"
#include "uniform_data.h"
#include <vector>
#include <map>

struct render_material
{
	std::map<std::string, uniform_data> uniforms;
	std::vector<render_texture*>		textures;
};

#endif