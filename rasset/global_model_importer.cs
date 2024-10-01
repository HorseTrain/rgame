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
    public class global_model_importer
    {
        public static model import_model(string file)
        {
            if (file.EndsWith(".dae"))
            {
                XmlSerializer s = new XmlSerializer(typeof(Collada));
                
                return collada.model_importer.load_from_collada((Collada) s.Deserialize(new XmlTextReader(file)));
            }
            else
            {
                throw new Exception();
            }
        }
    }
}
