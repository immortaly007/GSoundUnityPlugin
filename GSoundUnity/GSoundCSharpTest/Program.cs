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
        [DllImport("GSoundUnity.dll", EntryPoint = "?playSound@@YAXPBD@Z", CallingConvention = CallingConvention.Cdecl)]
        public static extern void playSound(string filename);

        [DllImport("GSoundUnity.dll", EntryPoint = "?init@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GSoundInit();

        [DllImport("GSoundUnity.dll", EntryPoint = "?stop@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GSoundStop();

        [DllImport("GSoundUnity.dll", EntryPoint = "?update@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GSoundUpdate();


        static void Main(string[] args)
        {
            //playSound("C:/acoustics.wav");

            GSoundInit();
            var time = new Stopwatch();
            time.Start();
            while(time.ElapsedMilliseconds < 5000)
            {
                Console.WriteLine("Updating sound..." + time.ElapsedMilliseconds);
                GSoundUpdate();
            }
            GSoundStop();
        }
    }
}
