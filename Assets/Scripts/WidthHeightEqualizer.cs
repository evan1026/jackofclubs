using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent (typeof(RectTransform))]
public class WidthHeightEqualizer : MonoBehaviour {

	public enum ChangeType {
		WIDTH,
		HEIGHT,
		BOTH
	}

	public ChangeType changeType;

	private RectTransform rect;
	private Vector2 offsetMaxInitial;
	private Vector2 offsetMinInitial;

	void Start() {
		rect = GetComponent<RectTransform> ();
		offsetMaxInitial = rect.offsetMax;
		offsetMinInitial = rect.offsetMin;
	}
	
	// Update is called once per frame
	void Update () {
		rect.offsetMax = offsetMaxInitial;
		rect.offsetMin = offsetMinInitial;

		Vector3[] corners = new Vector3[4];
		rect.GetLocalCorners(corners);

		int width = -1;
		int height = -1;
		for (int i = 0; i < corners.Length - 1; ++i) {
			Vector3 diff = corners [i + 1] - corners [i];

			if (diff.x != 0)
				width = (int)Mathf.Abs (diff.x);
			if (diff.y != 0)
				height = (int)Mathf.Abs (diff.y);
		}

		if (changeType == ChangeType.WIDTH || (changeType == ChangeType.BOTH && height < width)) {
			float offset = (width - height) / 2;
			rect.offsetMax += new Vector2 (-offset, 0);
			rect.offsetMin += new Vector2 (offset, 0);
		} else if (changeType == ChangeType.HEIGHT || (changeType == ChangeType.BOTH && width < height)) {
			float offset = (height - width) / 2;
			rect.offsetMax += new Vector2 (0, -offset);
			rect.offsetMin += new Vector2 (0, offset);
		}
	}
}
