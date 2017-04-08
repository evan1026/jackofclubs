using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

[Serializable]
public class SerializableColor {

	public float r;
	public float g;
	public float b;
	public float a;

	public SerializableColor(Color c) {
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
	}

	public Color getColor() {
		return new Color (r, g, b, a);
	}
}
