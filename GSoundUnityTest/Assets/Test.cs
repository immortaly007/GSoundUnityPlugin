using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System.Threading;

public class Test : MonoBehaviour {

	[DllImport("GSoundUnity.dll", EntryPoint = "?playSound@@YAXPBD@Z", CallingConvention = CallingConvention.Cdecl)]
	public static extern void PlaySound(string filename);

	[DllImport("GSoundUnity.dll", EntryPoint = "?getSomeInt@@YAHXZ")]
	public static extern int getSomeInt();

    [DllImport("GSoundUnity.dll", EntryPoint = "?init@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
    public static extern void GSoundInit();

    [DllImport("GSoundUnity.dll", EntryPoint = "?stop@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
    public static extern void GSoundStop();

    [DllImport("GSoundUnity.dll", EntryPoint = "?update@@YAXXZ", CallingConvention = CallingConvention.Cdecl)]
    public static extern void GSoundUpdate();
	
	// Use this for initialization
	void Start () {
        GSoundInit();
	}
	
	// Update is called once per frame
	void Update () {
        GSoundUpdate();
	}

    void OnApplicationQuit()
    {
        GSoundStop();
    }
}
