using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{
    public class material
    {
        public Dictionary<string, material_attribute> material_data { get; set; }

        public material()
        {
            material_data = new Dictionary<string, material_attribute>();
        }
    }
}
