
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace rasset
{
    public static class program
    {
        public static void Main()
        {
            model test_model = global_model_importer.import_model("D:\\smod\\model\\mario.dae");

            File.WriteAllBytes(@"D:\smod\model\model.rasset", test_model.export());
        }
    }
}
