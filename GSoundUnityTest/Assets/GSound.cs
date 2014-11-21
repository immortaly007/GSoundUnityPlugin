using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

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
}