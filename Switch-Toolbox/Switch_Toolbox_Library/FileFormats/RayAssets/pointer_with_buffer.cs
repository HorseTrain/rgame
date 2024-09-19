using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Toolbox.Library.FileFormats.RayAssets
{
    public class pointer_with_buffer
    {
        public int              pointer_location;
        public writable_buffer  buffer;

        public pointer_with_buffer()
        {
            buffer = new writable_buffer(); 
        }
    }
}
