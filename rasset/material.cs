using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{
    public class material
    {
        public string name                                          { get; set; }
        public Dictionary<string, material_attribute> material_data { get; set; }

        public material(string name)
        {
            material_data = new Dictionary<string, material_attribute>();
            this.name = name;
        }
    }
}
