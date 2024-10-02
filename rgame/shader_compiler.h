#pragma once

#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include <string>

struct io;

void compile_shader_source(std::string* result, std::string source, io* io_context);

#endif