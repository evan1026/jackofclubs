using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Text))]
public class SliderTextUpdater : MonoBehaviour {

	public void setValue(Slider s) {
		GetComponent<Text> ().text = s.value.ToString ();
	}
}
