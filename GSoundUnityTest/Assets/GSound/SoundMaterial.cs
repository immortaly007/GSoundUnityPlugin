using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GSound
{
    public class SoundMaterial
    {
        public float[] ReflectionAttenuation { get; private set; }

        public float TranRolloffFq { get; set; }
        public float TranRolloffSpeed { get; set; }
        public float AbsRollOff { get; set; }

        public SoundMaterial(float gain67, float gain125, float gain250, float gain500,
            float gain1000, float gain2000, float gain4000, float gain8000,
            float tranRolloffFq, float tranRolloffSpeed,
            float absRolloff)
        {
            ReflectionAttenuation = new float[] { gain67, gain125, gain250, gain500, gain1000, gain2000, gain4000, gain8000 };
            TranRolloffFq = tranRolloffFq;
            TranRolloffSpeed = tranRolloffSpeed;
            AbsRollOff = absRolloff;
        }

        public SoundMaterial(Material material)
        {
            switch (material)
            {
                case Material.Brick:
                    ReflectionAttenuation = new float[] { 0.98f, 0.98f, 0.98f, 0.97f, 0.96f, 0.95f, 0.93f, 0.92f };
                    TranRolloffFq = 800;
                    TranRolloffSpeed = 0.9f;
                    AbsRollOff = 0.5f;
                    break;
                case Material.Plywood:
                    ReflectionAttenuation = new float[] { 0.66f, 0.72f, 0.78f, 0.83f, 0.91f, 0.90f, 0.89f, 0.87f };
                    TranRolloffFq = 800;
                    TranRolloffSpeed = 0.9f;
                    AbsRollOff = 0.5f;
                    break;
                case Material.Soil:
                    ReflectionAttenuation = new float[] { 0.90f, 0.85f, 0.75f, 0.60f, 0.45f, 0.40f, 0.40f, 0.36f };
                    TranRolloffFq = 800;
                    TranRolloffSpeed = 0.9f;
                    AbsRollOff = 0.5f;
                    break;
                default:
                    // I don't know...
                    break;
            }
        }
    }

    public enum Material
    {
        Brick, Plywood, Soil
    }
}
