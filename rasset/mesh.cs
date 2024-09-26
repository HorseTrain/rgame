using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{
    public class mesh
    {
        public primative_type type          { get; set; }
        public List<vertex> vertices        { get; set; }
        public List<ushort> indices         { get; set; }

        public bool has_weights             { get; set; }
        public skeleton ameture             { get; set; }
        public material assigned_material   { get; set; }
    }
}
