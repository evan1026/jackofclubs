using UnityEngine;
using ProtoBuf;

[ProtoContract]
public class SerializableVector3 {

	[ProtoMember(1)]
	public float x;

	[ProtoMember(2)]
	public float y;

	[ProtoMember(3)]
	public float z;

	public SerializableVector3(Vector3 v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	public SerializableVector3() : this(new Vector3(0,0,0)) {}

	public Vector3 getVector3() {
		return new Vector3 (x, y, z);
	}
}
