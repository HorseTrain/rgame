using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{
    public class bone
    {
        public string name                              { get; set; }
        public int id                                   { get; set; }

        public Matrix4x4    local_transform             { get; set; }
        public Matrix4x4    inverse_world_transform     { get; set; }

        public bone parent_bone                         { get; set; }
        public bone child_bone                          { get; set; }
    }
}
