using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Image))]
public class ColorPreviewImageUpdater : MonoBehaviour {

	public enum ComponentType { RED, GREEN, BLUE };

	public void setColor(Color c) {
		GetComponent<Image> ().color = c;
	}

	public void setComponentFromSlider(Slider s, ComponentType c) {
		Color color = GetComponent<Image> ().color;
		float newVal = s.value / 255f;

		if (c == ComponentType.RED) {
			color.r = newVal;
		} else if (c == ComponentType.GREEN) {
			color.g = newVal;
		} else {
			color.b = newVal;
		}

		GetComponent<Image> ().color = color;
	}

	public void setRedFromSlider(Slider s) {
		setComponentFromSlider (s, ComponentType.RED);
	}

	public void setGreenFromSlider(Slider s) {
		setComponentFromSlider (s, ComponentType.GREEN);
	}

	public void setBlueFromSlider(Slider s) {
		setComponentFromSlider (s, ComponentType.BLUE);
	}
}
