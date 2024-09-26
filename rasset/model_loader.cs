using ColladaSchema;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;

namespace rasset
{
    public class model_loader
    {
        static model load_from_collada(Collada collada_file)
        {
            //TODO:

            throw new Exception();
        }

        public static model import_model(string file)
        {
            if (file.EndsWith(".dae"))
            {
                XmlSerializer s = new XmlSerializer(typeof(Collada));
                
                return load_from_collada((Collada) s.Deserialize(new XmlTextReader(file)));
            }
            else
            {
                throw new Exception();
            }
        }

        public static void Main()
        {
            import_model("D:\\smod\\model\\mario.dae");
        }
    }
}
