#pragma once

#include "wavefront.h"

#include "basic_render_mesh.h"
#include "string_tools.h"
#include "render_window.h"

static int convert_face_count_to_gl_type(int face_count)
{
	switch (face_count)
	{
	case 3: return GL_TRIANGLES;
	case 4: return GL_QUADS;
	default: assert(false); throw 0;
	}
}

template<typename T>
static void set_vertex_data_if_exists(T* result, std::string source_name, int* index_data, int face_index, std::map<std::string, std::vector<glm::vec4>>* vertex_data)
{
	int source_index = index_data[face_index] - 1;

	glm::vec4 vertex = (*vertex_data)[source_name][source_index];

	*result = *(T*)&vertex;
}

int wavefront_parse_mesh_object(basic_render_mesh* result_mesh, std::vector<std::string>* lines, std::map<std::string, std::vector<glm::vec4>>* carry_over_vertex_data, int line_location)
{
	std::vector<int>								source_face_data;

	int source_face_count = -1;

	for (; line_location < lines->size(); ++line_location)
	{
		std::string line = (*lines)[line_location];

		std::vector<std::string> line_parts;
		split_string(&line_parts, line, ' ');

		if (line_parts[0] == "f")
		{
			int working_face_count = line_parts.size() - 1;

			if (source_face_count == -1)
			{
				source_face_count = working_face_count;
			}
			else
			{
				if (source_face_count != working_face_count)
				{
					assert(false);

					throw 0;
				}
			}

			for (int i = 1; i < line_parts.size(); ++i)
			{
				std::vector<std::string> face_data_source;
				split_string(&face_data_source, line_parts[i], '/');

				for (int f = 0; f < face_data_source.size(); ++f)
				{
					source_face_data.push_back(std::stoi(face_data_source[f]));
				}
			}
		}
		else if (line_parts[0] == "o")
		{
			break;
		}
		else if (line_parts[0] == "mtllib" || line_parts[0] == "usemtl" || line_parts[0] == "s")
		{
			//
		}
		else
		{
			std::string attribute_name = line_parts[0];

			glm::vec4 working_vector;

			for (int i = 1; i < line_parts.size(); ++i)
			{
				float working_float = std::stof(line_parts[i]);

				working_vector[i - 1] = working_float;
			}

			(*carry_over_vertex_data)[attribute_name].push_back(working_vector);
		}
	}

	int primitive_point_count = source_face_count;
	int vertex_attribute_count = carry_over_vertex_data->size();
	int single_face_size = (primitive_point_count * vertex_attribute_count);

	if (source_face_data.size() % single_face_size != 0)
	{
		assert(false);

		throw 0;
	}

	int faces_count = source_face_data.size() / (primitive_point_count * vertex_attribute_count);

	basic_render_mesh::create(result_mesh, convert_face_count_to_gl_type(source_face_count));

	for (int face = 0; face < faces_count; ++face)
	{
		for (int point_data = 0; point_data < primitive_point_count; ++point_data)
		{
			int* working_face_data = source_face_data.data() + (face * single_face_size) + (point_data * vertex_attribute_count);

			basic_render_vertex new_vertex;

			set_vertex_data_if_exists(&new_vertex.position, "v", working_face_data, 0, carry_over_vertex_data);
			set_vertex_data_if_exists(&new_vertex.normal, "vn", working_face_data, 2, carry_over_vertex_data);
			set_vertex_data_if_exists(&new_vertex.uv, "vt", working_face_data, 1, carry_over_vertex_data);

			result_mesh->vertices.push_back(new_vertex);
		}

		for (int i = 0; i < source_face_count; ++i)
		{
			int index = result_mesh->indices.size();

			result_mesh->indices.push_back(index);
		}
	}

	return line_location;
}