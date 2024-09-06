#pragma once

#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>
#include <vector>

void io_read_entire_file(char** result, uint64_t* result_size, std::string file_path);
void io_read_entire_file_lines(std::vector<std::string>* result, std::string file_path);

#endif