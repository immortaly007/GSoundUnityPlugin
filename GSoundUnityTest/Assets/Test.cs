using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System.Threading;

public class Test : MonoBehaviour {
	
	// Use this for initialization
	void Start () {
        GSound.GSound.Init();
        GSound.GSound.AddSource(Application.dataPath + "/Sounds/acoustics.wav", 0, 1, -3, 1);
	}
	
	// Update is called once per frame
	void Update () {
        GSound.GSound.Update();
	}

    void OnApplicationQuit()
    {
        GSound.GSound.Stop();
        //GSound.Clear();
    }
}
