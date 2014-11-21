using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class Test : MonoBehaviour {

	[DllImport("GSoundUnity.dll", EntryPoint = "?playSound@@YAXXZ")]
	public static extern void playSound();

	[DllImport("GSoundUnity.dll", EntryPoint = "?getSomeInt@@YAHXZ")]
	public static extern int getSomeInt();
	
	// Use this for initialization
	void Start () {
		Debug.Log (getSomeInt ());
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
