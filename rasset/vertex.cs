using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{
    public struct Vector4i
    {
        public int x, y, z, w;
    }

    public class vertex
    {
        public Vector2  uv0;
        public Vector2  uv1;
        public Vector2  uv2;

        public Vector3  position;
        public Vector3  normal;
        public Vector3  tangent;
        public Vector3  bitangent;

        public Vector4  color;
        public Vector4  weights;
        public Vector4i weight_indices;
    }
}
