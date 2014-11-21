using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System.Threading;

public class Test : MonoBehaviour {
	
	// Use this for initialization
	void Start () {
        GSound.Init();
        GSound.AddSource(Application.dataPath + "/Sounds/acoustics.wav", 0, 1, -3, 1);
	}
	
	// Update is called once per frame
	void Update () {
        GSound.Update();
	}

    void OnApplicationQuit()
    {
        GSound.Stop();
        //GSound.Clear();
    }
}
