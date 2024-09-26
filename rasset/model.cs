using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{
    public class model
    {
        public Dictionary<string, mesh> mesehs  { get; set; }
        public skeleton ameture                 { get; set; }

        public model()
        {
            ameture = new skeleton();
            mesehs = new Dictionary<string, mesh>();
        }
    }
}
