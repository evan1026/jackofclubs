using UnityEngine;
using ProtoBuf;

[ProtoContract]
public class WorldMetaData {
	
	[ProtoMember(1)]
	public SerializableVector3 dimension;

}
