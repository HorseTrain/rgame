﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Threading.Tasks;
using GL_EditorFramework.EditorDrawables;
using OpenTK;
using Toolbox.Library;

namespace FirstPlugin.Turbo.CourseMuuntStructs
{
    public class LapPathGroup : BasePathGroup, IObject
    {
        public const string N_LapPathGroup = "LapPathGroup";
        public const string N_ReturnPointsError = "ReturnPointsError";
        public const string N_UnitIdNum = "UnitIdNum";

        public List<ReturnPoint> ReturnPoints = new List<ReturnPoint>();

        public LapPathGroup(dynamic bymlNode)
        {
            if (bymlNode is Dictionary<string, dynamic>) Prop = (Dictionary<string, dynamic>)bymlNode;
            else throw new Exception("Not a dictionary");

            int index = 0;
            foreach (var point in this["PathPt"])
            {
                PathPoints.Add(new LapPathPoint(point) { Name = $"Point [{index++}]" });
            }

            index = 0;
            foreach (var point in this["ReturnPoints"])
            {
                ReturnPoints.Add(new ReturnPoint(point) { Name = $"Point [{index++}]" });
            }
        }

        [Browsable(false)]
        public Dictionary<string, dynamic> Prop { get; set; } = new Dictionary<string, dynamic>();

        public int LapPathGroupId
        {
            get { return this[N_LapPathGroup]; }
            set { this[N_LapPathGroup] = value; }
        }

        public bool ReturnPointsError
        {
            get { return this[N_ReturnPointsError]; }
            set { this[N_ReturnPointsError] = value; }
        }

        public int UnitIdNum
        {
            get { return this[N_UnitIdNum]; }
            set { this[N_UnitIdNum] = value; }
        }

        public dynamic this[string name]
        {
            get
            {
                if (Prop.ContainsKey(name)) return Prop[name];
                else return null;
            }
            set
            {
                if (Prop.ContainsKey(name)) Prop[name] = value;
                else Prop.Add(name, value);
            }
        }
    }

    public class LapPathPoint : BasePathPoint
    {
        private RenderablePathPoint renderablePoint;

        public override bool IsHit(float X, float Y)
        {
            //Here we need to detect 2 points and see if the space between is selected
            var point1 = Matrix2DHelper.RotatePoint(Translate, Scale.X / 2, 44, Scale.Y / 2, Rotate);
            var point2 = Matrix2DHelper.RotatePoint(Translate, -(Scale.X / 2), -44, -(Scale.Y / 2), Rotate);


            float LeftPoint = point2.X;
            float RightPoint = point1.X;
            float BottomPoint = point2.Y;
            float TopPoint = point1.Y;

            bool isInBetweenX = (X > LeftPoint) && (X < RightPoint) ||
                                (X < LeftPoint) && (X > RightPoint);

            bool isInBetweenY = (Y > BottomPoint) && (Y < TopPoint) ||
                                (Y < BottomPoint) && (Y > TopPoint);

            if (isInBetweenX && isInBetweenY)
                return true;
            else
                return false;
        }

        [Browsable(false)]
        public override RenderablePathPoint RenderablePoint
        {
            get
            {
                if (renderablePoint == null)
                    renderablePoint = new RenderablePathPoint(new Vector4(0f, 0.25f, 1f, 0.1f), Translate, Rotate, Scale, this);
                return renderablePoint;
            }
        }

        [Category("Properties")]
        public int CheckPoint
        {
            get { return this["CheckPoint"] != null ? this["CheckPoint"] : -255; }
            set { if (value != -255) this["CheckPoint"] = value; }
        }

        [Category("Properties")]
        public int ClipIdx
        {
            get { return this["ClipIdx"] != null ? this["ClipIdx"] : -255; }
            set { if (value != -255) this["ClipIdx"] = value; }
        }

        [Category("Properties")]
        public bool HeadLightSW
        {
            get { return this["HeadLightSW"] != null ? this["HeadLightSW"] : false; }
            set { this["HeadLightSW"] = value; }
        }

        [Category("Properties")]
        public int LapCheck
        {
            get { return this["LapCheck"] != null ? this["LapCheck"] : -255; }
            set { if (value != -255)  this["LapCheck"] = value; }
        }

        [Category("Properties")]
        public int MapCameraFovy
        {
            get { return this["MapCameraFovy"] != null ? this["MapCameraFovy"] : -255; }
            set { if (value != -255) this["MapCameraFovy"] = value; }
        }

        [Category("Properties")]
        public int MapCameraY
        {
            get { return this["MapCameraY"] != null ? this["MapCameraY"] : -255; }
            set { if (value != -255) this["MapCameraY"] = value; }
        }

        public LapPathPoint(dynamic bymlNode)
        {
            if (bymlNode is Dictionary<string, dynamic>) Prop = (Dictionary<string, dynamic>)bymlNode;
            else throw new Exception("Not a dictionary");

            foreach (var point in this["NextPt"])
            {
                NextPoints.Add(new PointID(point));
            }
            foreach (var point in this["PrevPt"])
            {
                PrevPoints.Add(new PointID(point));
            }
        }
    }

}
