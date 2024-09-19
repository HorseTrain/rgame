using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Toolbox.Library.FileFormats.RayAssets
{
    public class r_file
    {
        public List<pointer_with_buffer>    all_pointers;
        public writable_buffer              buffer;

        public r_file()
        {
            buffer = new writable_buffer();
            all_pointers = new List<pointer_with_buffer>();
        }

        public pointer_with_buffer create_and_mark_pointer()
        {
            pointer_with_buffer result = new pointer_with_buffer();

            all_pointers.Add(result);

            result.pointer_location = buffer.location;
            buffer.write<int>(0);

            return result;
        }

        public byte[] get_export()
        {
            writable_buffer working_buffer = new writable_buffer(buffer);

            foreach (pointer_with_buffer pointer in all_pointers)
            {
                int real_location = working_buffer.write(pointer.buffer);

                working_buffer.seek(pointer.pointer_location);
                working_buffer.write(real_location - pointer.pointer_location);

                working_buffer.seek_to_end();
            }

            return working_buffer.raw_buffer.ToArray();
        }
    }
}
