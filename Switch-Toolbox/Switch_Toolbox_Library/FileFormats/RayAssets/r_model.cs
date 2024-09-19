using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Toolbox.Library.FileFormats.RayAssets;
using Toolbox.Library.Rendering;
using static Toolbox.Library.DAE;

namespace Toolbox.Library.FileFormats
{
    public static class r_model
    {
        static ulong create_mask(params bool[] flags)
        {
            if (flags.Length > 64)
            {
                throw new Exception();
            }

            ulong result = 0;

            for (int i = 0; i < flags.Length; i++)
            {
                if (flags[i])
                    result |= (1UL << i);
            }

            return result;
        }

        static void export_verticies(r_file model_file, STGenericObject generic_object)
        {

        }

        static void export_st_generic_object_meta(r_file model_file, STGenericObject generic_object)
        {
            writable_buffer raw_result = model_file.buffer;

            pointer_with_buffer meta_data_pointer = model_file.create_and_mark_pointer();

            writable_buffer meta_data_buffer = meta_data_pointer.buffer;

            meta_data_buffer.write(
                create_mask(
                    generic_object.HasPos,
                    generic_object.HasNrm,
                    generic_object.HasUv0,
                    generic_object.HasUv1,
                    generic_object.HasUv2,
                    generic_object.HasWeights,
                    generic_object.HasIndices,
                    generic_object.HasBitans,
                    generic_object.HasTans,
                    generic_object.HasVertColors,
                    generic_object.HasVertColors2
                    )
            );

            meta_data_buffer.write(generic_object.BoneIndex);
            meta_data_buffer.write(generic_object.MaterialIndex);
            meta_data_buffer.write(generic_object.VertexBufferIndex);
            meta_data_buffer.write(generic_object.DisplayLODIndex);
            meta_data_buffer.write(generic_object.Offset);

            if (generic_object.lodMeshes.Count != 1)
            {
                throw new Exception();
            }

            meta_data_buffer.write(generic_object.lodMeshes[0].PrimativeType);

            raw_result.write(generic_object.vertices.Count());
            pointer_with_buffer vertex_data_pointer = model_file.create_and_mark_pointer();
            writable_buffer vertex_data = vertex_data_pointer.buffer;

            foreach (Vertex v in generic_object.vertices)
            {
                vertex_data.write(v.pos);
                vertex_data.write(v.nrm);
                vertex_data.write(v.col);
                vertex_data.write(v.col2);

                vertex_data.write(v.uv0);
                vertex_data.write(v.uv1);
                vertex_data.write(v.uv2);
                vertex_data.write(v.uv3);

                vertex_data.write(v.tan);
                vertex_data.write(v.bitan);

                vertex_data.write((byte)v.boneIds.Count);

                for (int i = 0; i < v.boneIds.Count; ++i)
                {
                    vertex_data.write((ushort)v.boneIds[i]);
                    vertex_data.write(v.boneWeights[i]);
                }
            }

            raw_result.write(generic_object.faces.Count());
            pointer_with_buffer face_data_pointer = model_file.create_and_mark_pointer();
            writable_buffer face_data = face_data_pointer.buffer;

            foreach (int i in generic_object.faces)
            {
                face_data.write((ushort)i);
            }
        }

        public static void export_model(string FileName, ExportSettings settings, STGenericModel model, List<STGenericTexture> Textures, STSkeleton skeleton = null, List<int> NodeArray = null)
        {
            r_file model_file = new r_file();
            writable_buffer result = model_file.buffer;

            result.write(model.Objects.Count());

            foreach (STGenericObject model_object in model.Objects)
            {
                export_st_generic_object_meta(model_file, model_object);
            }

            File.WriteAllBytes(FileName, model_file.get_export());
        }
    }
}
