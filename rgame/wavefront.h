#pragma once

#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include <string>
#include <vector>
#include <map>

#include "glm/vec4.hpp"

struct basic_render_mesh;

int wavefront_parse_mesh_object(basic_render_mesh* result_mesh, std::vector<std::string>* lines, std::map<std::string, std::vector<glm::vec4>>* carry_over_vertex_data, int line_location);

#endif