using UnityEngine;
using ProtoBuf;

[ProtoContract]
public class SerializableColor {

	[ProtoMember(1)]
	public float r;

	[ProtoMember(2)]
	public float g;

	[ProtoMember(3)]
	public float b;

	[ProtoMember(4)]
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
