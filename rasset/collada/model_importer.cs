using ColladaSchema;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace rasset.collada
{
    internal class model_importer
    {
        public static model load_from_collada(Collada collada_file)
        {
            //TODO:

            model result = new model();

            //load_materials(result, collada_file);
            load_geometries(result, collada_file);

            return result;
        }

        static void load_materials(model result, Collada collada_file)
        {
            LibraryMaterials working_materials = collada_file.LibraryMaterials.First();

            if (working_materials == null)
            {
                return;
            }

            Dictionary<string, string> effect_paths = new Dictionary<string, string>();

            //TODO:
            foreach (Material working_material in working_materials.Material)
            {
                string material_name = working_material.Id;

                result.materials.Add(material_name, new material(material_name));

                if (working_material.InstanceEffect != null)
                {
                    effect_paths.Add(material_name, working_material.InstanceEffect.Url);
                }
            }
        }

        static List<float> load_mesh_source(Source source, out string id)
        {
            id = source.Id;

            FloatArray float_array_source = source.FloatArray.First();

            StringBuilder array_source = new StringBuilder(float_array_source.Value);

            while (true)
            {
                int o_count = array_source.Length;

                array_source.Replace("  ", " ");
                array_source.Replace("\n", " ");

                if (o_count == array_source.Length)
                    break;
            }

            string[] parts = array_source.ToString().Split(' ');

            List<float> result = new List<float>();

            foreach (string part in parts)
            {
                if (!float.TryParse(part, out float value))
                    continue;

                result.Add(value );
            }

            if ((ulong)result.Count != float_array_source.Count)
            {
                throw new Exception();
            }

            return result;
        }

        static Vector2 get_vector_2(List<float> source, int offset)
        {
            return new Vector2(source[(offset * 2) + 0], source[(offset * 2) + 1]);
        }

        static Vector3 get_vector_3(List<float> source, int offset)
        {
            return new Vector3(source[(offset * 3) + 0], source[(offset * 3) + 1], source[(offset * 3) + 2]);
        }

        static Vector4 get_vector_4(List<float> source, int offset)
        {
            return new Vector4(source[(offset * 4) + 0], source[(offset * 4) + 1], source[(offset * 4) + 2], source[(offset * 4) + 3]);
        }

        static List<int> load_mesh_indices(Triangles triangles, int input_semantic_count)
        {
            List<int> result = new List<int>();

            if (triangles.P.Count != 1)
            {
                throw new Exception();
            }

            string p_data = triangles.P.First();

            string[] source = p_data.Split(' ');

            foreach (string s in source)
            {
                if (!int.TryParse(s, out int v))
                    continue;

                result.Add(v);
            }

            if (result.Count != (int)triangles.Count * input_semantic_count * 3)
                throw new Exception();

            return result;
        }

        static vertex create_vertex_from_semantic(List<int> triangle_data, int first_semantic_offset, Dictionary<string, InputLocalOffset> semantics, Dictionary<string, List<float>> vertex_data)
        {
            vertex result = new vertex();

            foreach (var semantic in semantics)
            {
                List<float> working_data = vertex_data[semantic.Value.Source.Substring(1)];

                switch (semantic.Key)
                {
                    case "VERTEX_0": result.position = get_vector_3(working_data, triangle_data[first_semantic_offset + (int)semantic.Value.Offset]); break;
                    case "NORMAL_0": result.normal = get_vector_3(working_data, triangle_data[first_semantic_offset + (int)semantic.Value.Offset]); break;
                    case "COLOR_0": result.color = get_vector_4(working_data, triangle_data[first_semantic_offset + (int)semantic.Value.Offset]); break;
                    case "TEXCOORD_0": result.uv0 = get_vector_2(working_data, triangle_data[first_semantic_offset + (int)semantic.Value.Offset]); break;
                    case "TEXCOORD_1": result.uv1 = get_vector_2(working_data, triangle_data[first_semantic_offset + (int)semantic.Value.Offset]); break;
                    case "TEXCOORD_2": result.uv2 = get_vector_2(working_data, triangle_data[first_semantic_offset + (int)semantic.Value.Offset]); break;
                }
            }

            return result;
        }

        static void load_geometry(model result, Geometry geometry_data)
        {
            mesh result_mesh = new mesh();

            result_mesh.name = geometry_data.Name;

            Dictionary<string, List<float>> vertex_data = new Dictionary<string, List<float>>();
            Dictionary<string, InputLocalOffset> input_semantics = new Dictionary<string, InputLocalOffset>();

            foreach (Source s in geometry_data.Mesh.Source)
            {
                List<float> float_array = load_mesh_source(s, out string id);

                vertex_data.Add(id, float_array);
            }

            if (geometry_data.Mesh.Vertices != null)
            {
                Vertices data = geometry_data.Mesh.Vertices;

                foreach (InputLocal i in data.Input)
                {
                    vertex_data.Add(data.Id, vertex_data[i.Source.Substring(1)]);
                }
            }

            List<vertex> vertices = new List<vertex>();
            List<ushort> indices = new List<ushort>();

            result_mesh.vertices = vertices;
            result_mesh.indices = indices;

            if (geometry_data.Mesh.Triangles.Count == 1)
            {
                foreach (InputLocalOffset input_data in geometry_data.Mesh.Triangles.First().Input)
                {
                    string semantic_name = $"{input_data.Semantic}_{input_data.Set}";

                    input_semantics.Add(semantic_name, input_data);
                }

                result_mesh.type = primative_type.triangle;

                List<int> trianlge_source = load_mesh_indices(geometry_data.Mesh.Triangles.First(), input_semantics.Keys.Count);

                int semantic_count = input_semantics.Count;
                int triangle_count = trianlge_source.Count / semantic_count;

                for (int i = 0; i < triangle_count; ++i)
                {
                    int offset = i * semantic_count;

                    vertex working_vertex = create_vertex_from_semantic(trianlge_source, offset, input_semantics, vertex_data);

                    vertices.Add(working_vertex);
                    indices.Add((ushort)indices.Count);
                }
            }
            else
            {
                throw new Exception();
            }

            result.meshes.Add(result_mesh.name,result_mesh);
        }

        static void load_geometries(model result, Collada collada_file)
        {
            foreach (LibraryGeometries geomatry_libary in collada_file.LibraryGeometries)
            {
                foreach (Geometry geomatry in geomatry_libary.Geometry)
                {
                    load_geometry(result, geomatry);
                }
            }
        }
    }
}
