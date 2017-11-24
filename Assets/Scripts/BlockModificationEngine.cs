using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityStandardAssets.Characters.FirstPerson;

public class BlockModificationEngine : MonoBehaviour {

	void Update () {
		if (FirstPersonController.localInstance.getCursorLock ()) {
			if (Input.GetMouseButtonDown (0)) {
				Debug.Log ("CLICKED");
				RaycastHit selectedBlock = RayCasting.Instance.getSelectedBlock ();
				if (selectedBlock.collider != null) {
					TerrainHelper.SetBlock (selectedBlock, new BlockAir ());
				}
			} else if (Input.GetMouseButtonDown (1)) {
				Debug.Log ("CLICKED");
				RaycastHit selectedBlock = RayCasting.Instance.getSelectedBlock ();
				if (selectedBlock.collider != null) {
					TerrainHelper.SetBlock (selectedBlock, new Block (ColorSelection.Instance.getColor ()), true);
				}
			}
		}
	}
}
