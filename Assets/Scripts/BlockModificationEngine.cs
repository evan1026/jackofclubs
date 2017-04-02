using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlockModificationEngine : MonoBehaviour {

	public float maxDistance;
	public static int worldLayer = 8;
	private int layerMask;

	void Start() {
		layerMask = 1 << worldLayer;
	}

	void Update () {
		if (Input.GetMouseButtonDown (0)) {
			RaycastHit hit;
			if (Physics.Raycast (transform.position, transform.forward, out hit, maxDistance, layerMask)) {
				TerrainHelper.SetBlock (hit, new BlockAir ());
			}
		} else if (Input.GetMouseButtonDown (1)) {
			RaycastHit hit;
			if (Physics.Raycast (transform.position, transform.forward, out hit, maxDistance, layerMask)) {
				TerrainHelper.SetBlock (hit, new Block (new Color32 (255, 255, 255, 255)), true);
			}
		}
	}
}
