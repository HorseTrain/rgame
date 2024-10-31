#pragma once

#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include <string>

struct io;
struct render_shader;

void compile_shader_source(std::string* result, std::string source, io* io_context);
void get_shader_from_path(render_shader* result, std::string path, io* io_context);

#endif