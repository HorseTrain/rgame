using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{
    public class mesh
    {
        public string name                  { get; set; }

        public primative_type type          { get; set; }
        public List<vertex> vertices        { get; set; }
        public List<ushort> indices         { get; set; }

        public bool has_weights             { get; set; }
        public skeleton ameture             { get; set; }
        public material assigned_material   { get; set; }

        public byte[] export()
        {
            exportable_r_asset result = new exportable_r_asset();

            result.write_string(name);
            result.write_struct(type);
            result.write_struct(has_weights ? 1 : 0);

            var vertex_buffer = result.create_buffer_at_top();
            exportable_r_asset.write_struct(vertex_buffer.data, vertices.Count);

            foreach (vertex v in vertices)
            {
                exportable_r_asset.write_struct(vertex_buffer.data, v.uv0);
                exportable_r_asset.write_struct(vertex_buffer.data, v.uv1);
                exportable_r_asset.write_struct(vertex_buffer.data, v.uv2);

                exportable_r_asset.write_struct(vertex_buffer.data, v.position);
                exportable_r_asset.write_struct(vertex_buffer.data, v.normal);
                exportable_r_asset.write_struct(vertex_buffer.data, v.tangent);
                exportable_r_asset.write_struct(vertex_buffer.data, v.bitangent);

                exportable_r_asset.write_struct(vertex_buffer.data, v.color);
                exportable_r_asset.write_struct(vertex_buffer.data, v.weight_indices);
                exportable_r_asset.write_struct(vertex_buffer.data, v.weights);
            }

            var index_buffer = result.create_buffer_at_top();
            exportable_r_asset.write_struct(index_buffer.data, indices.Count);

            foreach (ushort u in indices)
            {
                exportable_r_asset.write_struct(index_buffer.data, u);
            }

            return result.export();
        }
    }
}
