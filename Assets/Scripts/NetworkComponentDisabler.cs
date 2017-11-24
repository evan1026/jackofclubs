using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityStandardAssets.Characters.FirstPerson;

public class NetworkComponentDisabler : NetworkBehaviour {

	// Use this for initialization
	void Start () {
		if (!isLocalPlayer) {
			GetComponentInChildren<FirstPersonController> ().enabled = false;
			GetComponentInChildren<BlockModificationEngine> ().enabled = false;
			GetComponentInChildren<AudioListener> ().enabled = false;
			GetComponentInChildren<Camera> ().enabled = false;
		} else {
			var player = transform.Find ("Player");
			player.GetComponent<Collider> ().enabled = false;

			Renderer[] playermodel = player.GetComponentsInChildren<Renderer> ();
			foreach (Renderer renderer in playermodel) {
				renderer.enabled = false;
			}
		}
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
