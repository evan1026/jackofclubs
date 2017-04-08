using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(RectTransform))]
public class LoadingBarScript : MonoBehaviour {

	float percent;

	// Use this for initialization
	void Start () {
		setPercent (0f);
	}

	public void setPercent(float p) {
		percent = p;

		if (percent > 1)
			percent = 1f;

		if (percent < 0)
			percent = 0f;

		RectTransform rect = GetComponent<RectTransform> ();
		rect.anchorMax = new Vector2 (percent, 1);
	}
}
