using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlockModificationEngine : MonoBehaviour {

	void Update () {
		if (Input.GetMouseButtonDown (0)) {
			RaycastHit selectedBlock = RayCasting.Instance.getSelectedBlock ();
			if (selectedBlock.collider != null) {
				TerrainHelper.SetBlock (selectedBlock, new BlockAir ());
			}
		} else if (Input.GetMouseButtonDown (1)) {
			RaycastHit selectedBlock = RayCasting.Instance.getSelectedBlock ();
			if (selectedBlock.collider != null) {
				TerrainHelper.SetBlock (selectedBlock, new Block (ColorSelection.Instance.getColor()), true);
			}
		}
	}
}
