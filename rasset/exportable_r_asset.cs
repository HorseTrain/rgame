using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{

    public unsafe class exportable_r_asset
    {
        public class pointer
        {
            public int  pointer_offset;
            public int  data_location;

            public bool marked;
        }

        public class sub_buffer
        {
            public pointer      pointer_reference;
            public List<byte>   data;
        }

        public List<byte> result_buffer     { get; set; }
        public List<pointer> pointers       { get; set; }
        public List<sub_buffer> sub_buffers { get; set; }

        public exportable_r_asset()
        {
            result_buffer = new List<byte>();
            pointers = new List<pointer>();
            sub_buffers = new List<sub_buffer>();
        }

        public static int write_struct<T>(List<byte> result_buffer,T value) where T : unmanaged
        {
            int location = result_buffer.Count;

            for (int i = 0; i < sizeof(T); i++)
            {
                result_buffer.Add(((byte*)&value)[i]);
            }

            return location;
        }

        public int write_struct<T>(T value) where T : unmanaged
        {
            return write_struct<T>(result_buffer, value);
        }

        public pointer create_pointer_at_top()
        {
            pointer result = new pointer();

            result.pointer_offset = write_struct(0);
            result.data_location = 0;

            result.marked = false;

            pointers.Add(result);   

            return result;
        }

        public void mark_pointer(pointer to_mark)
        {
            if (to_mark.marked)
            {
                throw new Exception();
            }

            to_mark.marked = true;

            to_mark.data_location = result_buffer.Count;    
        }

        public sub_buffer create_buffer_at_top()
        {
            sub_buffer result = new sub_buffer();

            result.pointer_reference = create_pointer_at_top();
            result.data = new List<byte>();

            sub_buffers.Add(result);

            return result;
        }

        public sub_buffer create_buffer(byte[] buffer)
        {
            sub_buffer result = create_buffer_at_top();

            result.data = buffer.ToList();

            return result;
        }

        public void write_string(string to_write)
        {
            int size = to_write.Length;

            sub_buffer buffer = create_buffer_at_top();

            write_struct(buffer.data, size);

            for (int i = 0; i < to_write.Length; i++)
            {
                buffer.data.Add((byte)to_write[i]);
            }
        }

        public byte[] export()
        {
            foreach (sub_buffer buffer in sub_buffers)
            {
                mark_pointer(buffer.pointer_reference);

                result_buffer.AddRange(buffer.data);
            }

            byte[] working = result_buffer.ToArray();

            fixed (byte* working_raw  = working)
            {
                foreach (pointer p in pointers)
                {
                    *(int*)(working_raw + p.pointer_offset) = p.data_location - p.pointer_offset;
                }
            }

            return working;
        }

    }
}
