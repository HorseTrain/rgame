using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Toolbox.Library.FileFormats.RayAssets
{
    public unsafe class writable_buffer
    {
        public int          location;
        public List<byte>   raw_buffer;

        public writable_buffer()
        {
            location = 0;
            raw_buffer = new List<byte>();
        }

        public writable_buffer(writable_buffer source)
        {
            this.location = source.raw_buffer.Count;
            this.raw_buffer = source.raw_buffer.ToList();
        }

        public void seek(int new_location)
        {
            location = new_location;
        }

        public int write<T> (T data) where T: unmanaged
        {
            int result = location;

            for (int i = 0; i < sizeof(T); i++)
            {
                while (location >= raw_buffer.Count)
                    raw_buffer.Add(0);

                raw_buffer[location] = ((byte*)&data)[i];

                location++;
            }

            return result;
        }

        public int write(writable_buffer data)
        {
            return write(data.raw_buffer.ToArray());
        }

        public int write(byte[] data)
        {
            int result = location;

            raw_buffer.AddRange(data);
            location += data.Length;

            return result;
        }

        public void seek_to_end()
        {
            seek(raw_buffer.Count); 
        }
    }
}
