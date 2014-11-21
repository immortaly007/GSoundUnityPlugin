using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace GSound
{
    public class AABB
    {
        public float MinX { get; set; }
        public float MinY { get; set; }
        public float MinZ { get; set; }
        public float MaxX { get; set; }
        public float MaxY { get; set; }
        public float MaxZ { get; set; }

        public AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
        {
            MinX = minX;
            MaxX = maxX;
            MinY = minY;
            MaxY = maxY;
            MinZ = minZ;
            MaxZ = maxZ;
        }

        public AABB(Vector3 min, Vector3 max)
        {
            MinX = min.x;
            MaxX = max.x;
            MinY = min.y;
            MaxY = max.y;
            MinZ = min.z;
            MaxZ = max.z;
        }
    }
}
