using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{
    public class model
    {
        public Dictionary<string, mesh> meshes          { get; set; }
        public Dictionary<string, material> materials   { get; set; }
        public skeleton ameture                         { get; set; }

        public model()
        {
            ameture = new skeleton();
            meshes = new Dictionary<string, mesh>();
            materials = new Dictionary<string, material>();
        }

        public byte[] export()
        {
            exportable_r_asset result = new exportable_r_asset();

            result.write_struct(meshes.Count);

            foreach (var m in meshes.Values)
            {
                result.create_buffer(m.export());
            }

            return result.export();
        }
    }
}
