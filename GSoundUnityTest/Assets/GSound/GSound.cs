using GSound;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine;

namespace GSound
{
    public static class GSound
    {
        [DllImport("GSoundUnity.dll", EntryPoint = "?playSound@@YAXPBD@Z", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PlaySound(string filename);

        [DllImport("GSoundUnity.dll", EntryPoint = "?getSomeInt@@YAHXZ")]
        public static extern int getSomeInt();

        [DllImport("GSoundUnity.dll", EntryPoint = "?init@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Init();

        [DllImport("GSoundUnity.dll", EntryPoint = "?stop@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Stop();

        [DllImport("GSoundUnity.dll", EntryPoint = "?clear@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Clear();

        [DllImport("GSoundUnity.dll", EntryPoint = "?update@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Update();

        [DllImport("GSoundUnity.dll", EntryPoint = "?addSource@@YAXPBDMMMM@Z", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AddSource(string filename, float posX, float posY, float posZ, float volume);

        [DllImport("GSoundUnity.dll", EntryPoint = "?addAABB@@YAXMMMMMMMMMMMMMMMMM@Z", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AddAABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ,
            float gain67, float gain125, float gain250, float gain500,
            float gain1000, float gain2000, float gain4000, float gain8000,
            float tranRolloffFq, float tranRolloffSpeed,
            float absRolloff);

        public static void AddAABB(AABB box, SoundMaterial material)
        {
            AddAABB(box.MinX, box.MaxX, box.MinY, box.MaxY, box.MinZ, box.MaxZ,
                material.ReflectionAttenuation[0], material.ReflectionAttenuation[1], material.ReflectionAttenuation[2],
                material.ReflectionAttenuation[3], material.ReflectionAttenuation[4], material.ReflectionAttenuation[5],
                material.ReflectionAttenuation[6], material.ReflectionAttenuation[7],
                material.TranRolloffFq, material.TranRolloffSpeed, material.AbsRollOff);
        }
    }
}