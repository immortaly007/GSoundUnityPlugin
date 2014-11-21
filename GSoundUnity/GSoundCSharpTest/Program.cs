using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace GSoundCSharpTest
{
    class Program
    {
        [DllImport("GSoundUnity.dll", EntryPoint = "?playSound@@YAXXZ")]
        public static extern void playSound();

        static void Main(string[] args)
        {
            playSound();
        }
    }
}
