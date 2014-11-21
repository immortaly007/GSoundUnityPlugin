using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using GSound;

namespace GSoundCSharpTest
{
    class Program
    {

        static void Main(string[] args)
        {
            //playSound("C:/acoustics.wav");
            GSound.GSound.Init();
            //GSound.GSound.AddSource(Environment.CurrentDirectory + "/Data/acoustics.wav", 0, 1, -3, 1);
            GSound.GSound.AddSource("C:/acoustics.wav", 0, 1, -3, 1);
            var box = new AABB(-2, 2, 0, 3, -4, 4);
            var material = new SoundMaterial(Material.Plywood);
            GSound.GSound.AddAABB(box, material);
            var time = new Stopwatch();
            time.Start();
            while(time.ElapsedMilliseconds < 5000)
            {
                Console.WriteLine("Updating sound..." + time.ElapsedMilliseconds);
                GSound.GSound.Update();
            }
            GSound.GSound.Stop();
            //GSound.Clear();

            // Try if it can be run again real quick;
            GSound.GSound.Init();
            GSound.GSound.Update();
            GSound.GSound.Stop();
        }
    }
}
