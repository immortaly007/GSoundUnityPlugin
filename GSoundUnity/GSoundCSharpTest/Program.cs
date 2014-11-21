using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace GSoundCSharpTest
{
    class Program
    {

        static void Main(string[] args)
        {
            //playSound("C:/acoustics.wav");
            GSound.Init();
            GSound.AddSource("C:/acoustics.wav", 0, 1, -3, 1);
            var time = new Stopwatch();
            time.Start();
            while(time.ElapsedMilliseconds < 1000)
            {
                Console.WriteLine("Updating sound..." + time.ElapsedMilliseconds);
                GSound.Update();
            }
            GSound.Stop();
            //GSound.Clear();

            // Try if it can be run again real quick;
            GSound.Init();
            GSound.Update();
            GSound.Stop();
        }
    }
}
