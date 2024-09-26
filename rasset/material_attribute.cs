using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{
    public enum material_attribute_type
    {

    }

    public class material_attribute
    {
        public material_attribute_type type { get; set; }
        public string name                  { get; set; }
        public object data                  { get; set; }
    }
}
