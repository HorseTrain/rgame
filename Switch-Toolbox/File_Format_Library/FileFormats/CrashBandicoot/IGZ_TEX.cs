﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Toolbox;
using System.Windows.Forms;
using Toolbox.Library;
using Toolbox.Library.IO;
using Toolbox.Library.Forms;

namespace FirstPlugin
{
    //I keep textures seperate due to the way files are handled
    //While i could store all section data in one file, it's better to keep textures as 
    //a single tree node as all model sections are usually empty
    public class IGZ_TEX : STGenericTexture, IFileFormat, ISingleTextureIconLoader, IContextMenuNode
    {
        public STGenericTexture IconTexture { get { return this; } }

        public FileType FileType { get; set; } = FileType.Layout;

        public bool CanSave { get; set; }
        public string[] Description { get; set; } = new string[] { "Crash N.Sane Trilogy / CTR: Nitro-Fueled (IGZ)" };
        public string[] Extension { get; set; } = new string[] { "*.igz" };
        public string FileName { get; set; }
        public string FilePath { get; set; }
        public IFileInfo IFileInfo { get; set; }

        public bool Identify(System.IO.Stream stream)
        {
            return IGZ_Structure.Identify(stream);
        }

        public Type[] Types
        {
            get
            {
                List<Type> types = new List<Type>();
                return types.ToArray();
            }
        }

        public ToolStripItem[] GetContextMenuItems()
        {
            List<ToolStripItem> Items = new List<ToolStripItem>();
            Items.Add(new ToolStripMenuItem("Save", null, SaveAction, Keys.Control | Keys.S));
            Items.AddRange(base.GetContextMenuItems());
            return Items.ToArray();
        }


        private void SaveAction(object sender, EventArgs args)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = Utils.GetAllFilters(this);
            sfd.FileName = FileName;

            if (sfd.ShowDialog() == DialogResult.OK)
            {
                STFileSaver.SaveFileFormat(this, sfd.FileName);
            }
        }

        public IGZ_Structure IGZStructure;

        public IGZ_Structure.Platform PlatformFormat;

        public void Load(System.IO.Stream stream)
        {
            CanSave = true;
            CanRename = true;
            CanReplace = true;

            using (var reader = new FileReader(stream))
            {
                //Parse with the IGZ structure class and get the texture info block
                IGZStructure = new IGZ_Structure();
                IGZStructure.Read(reader);

                if (IGZStructure.TextureInfo == null)
                {
                    CanSave = false;
                    return;
                }

                //Set all the info from the parsed struct
                var texInfo = IGZStructure.TextureInfo;
                Width = texInfo.Width;
                Height = texInfo.Height;
                Format = texInfo.FormatInfo.Format;
                PlatformFormat = texInfo.FormatInfo.Platform;
                ArrayCount = texInfo.ArrayCount;
                Depth = texInfo.Depth;

                //Don't set the platform swizzle because they don't seem to be swizzled?
                //    PlatformSwizzle = texInfo.FormatInfo.Platform;

                //Grab the first entry from the string table
                if (IGZStructure.StringTable.Count > 0)
                    Text = IGZStructure.StringTable[0];
                else
                    Text = FileName;

                Parameters.FlipY = true;
            }
        }

        public override TEX_FORMAT[] SupportedFormats
        {
            get
            {
                return new TEX_FORMAT[]
                {
                    TEX_FORMAT.BC1_UNORM,
                    TEX_FORMAT.BC3_UNORM,
                    TEX_FORMAT.BC5_UNORM,
                    TEX_FORMAT.R8G8B8A8_UNORM,
                };
            }
        }

        public override void Replace(string FileName)
        {
            GenericTextureImporterList importer = new GenericTextureImporterList(SupportedFormats);
            GenericTextureImporterSettings settings = new GenericTextureImporterSettings();

            if (Utils.GetExtension(FileName) == ".dds" ||
                Utils.GetExtension(FileName) == ".dds2")
            {
                settings.LoadDDS(FileName);
                importer.LoadSettings(new List<GenericTextureImporterSettings>() { settings, });
                ApplySettings(settings);
                UpdateEditor();
            }
            else
            {
                settings.LoadBitMap(FileName);
                importer.LoadSettings(new List<GenericTextureImporterSettings>() { settings, });

                if (importer.ShowDialog() == DialogResult.OK)
                {
                    if (settings.GenerateMipmaps && !settings.IsFinishedCompressing)
                    {
                        settings.DataBlockOutput.Clear();
                        settings.DataBlockOutput.Add(settings.GenerateMips(importer.CompressionMode, importer.MultiThreading));
                    }

                    ApplySettings(settings);
                    UpdateEditor();
                }
            }
        }

        private void ApplySettings(GenericTextureImporterSettings settings)
        {
            //Combine all arrays
            IGZStructure.TextureInfo.ImageData = Utils.CombineByteArray(
                settings.DataBlockOutput.ToArray());

            this.Width = settings.TexWidth;
            this.Height = settings.TexHeight;
            this.Format = settings.Format;
            this.MipCount = settings.MipCount;
            this.Depth = settings.Depth;
            this.ArrayCount = (uint)settings.DataBlockOutput.Count;
        }

        public override bool CanEdit { get; set; } = true;

        public override void SetImageData(System.Drawing.Bitmap bitmap, int ArrayLevel)
        {
            Console.WriteLine("Setting ImageData");
            IGZStructure.TextureInfo.ImageData = GenerateMipsAndCompress(bitmap, MipCount, Format);
        }

        public override byte[] GetImageData(int ArrayLevel = 0, int MipLevel = 0, int DepthLevel = 0)
        {
            if (PlatformSwizzle == PlatformSwizzle.Platform_Switch)
                return TegraX1Swizzle.GetImageData(this, IGZStructure.TextureInfo.ImageData, ArrayLevel, MipLevel, 1);
            else
                return IGZStructure.TextureInfo.ImageData;
        }

        public override void OnClick(TreeView treeView) {
            UpdateEditor();
        }

        private void UpdateEditor()
        {
            ImageEditorBase editor = (ImageEditorBase)LibraryGUI.GetActiveContent(typeof(ImageEditorBase));
            if (editor == null)
            {
                editor = new ImageEditorBase();
                editor.Dock = DockStyle.Fill;
                LibraryGUI.LoadEditor(editor);
            }

            editor.LoadProperties(GenericProperties);
            editor.LoadImage(this);
        }

        public void Unload()
        {

        }

        public void Save(System.IO.Stream stream)
        {
            //Apply the image info block from this image
            IGZStructure.TextureInfo.Width = (ushort)this.Width;
            IGZStructure.TextureInfo.Height = (ushort)this.Height;
            IGZStructure.TextureInfo.MipCount = (ushort)this.MipCount;
            IGZStructure.TextureInfo.Depth = (ushort)this.Depth;
            IGZStructure.TextureInfo.ArrayCount = (ushort)this.ArrayCount;
            IGZStructure.TextureInfo.FormatInfo.Format = this.Format;
            IGZStructure.TextureInfo.FormatInfo.Platform = this.PlatformFormat;
            IGZStructure.StringTable[0] = this.Text;

            IGZStructure.Write(new FileWriter(stream));
        }
    }
}
