
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
            model test_model = global_model_importer.import_model("C:\\Users\\student\\Documents\\GitHub\\Building\\Output\\smo\\smo_assets\\mario.dae");

            File.WriteAllBytes(@"C:\Users\student\Documents\GitHub\Building\Output\smo\assets\actor\mario\model.rasset", test_model.export());
        }
    }
}
