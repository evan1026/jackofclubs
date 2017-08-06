using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RayCasting : Singleton<RayCasting> {

	public float maxDistance;
	public static int worldLayer = 8;
	private int layerMask;

	// Use this for initialization
	void Start () {
		layerMask = 1 << worldLayer;
	}
	
	public RaycastHit getSelectedBlock() {
		RaycastHit hit;
		Physics.Raycast (transform.position, transform.forward, out hit, maxDistance, layerMask);
		return hit;
	}
}
