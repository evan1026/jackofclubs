using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using System.IO;
using ProtoBuf;
using System;
using System.Linq;

public class NetworkDataRequests : NetworkBehaviour {

	public static NetworkDataRequests Instance = null;

	private int lastSequence = 0;
	private byte[] _bytes;

	public static int maxMessageSize = 64000;

	private NetworkIdentity identity;

	// Use this for initialization
	void Start () {
		Instance = this;
		identity = GetComponent<NetworkIdentity> ();
	}

	[Command]
	public void CmdGetChunk(WorldPos worldPos) {
		World w = GetComponentInParent<World> ();
		Block[,,] blocks = World.Instance.GetChunk (worldPos.x, worldPos.y, worldPos.z).blocks;

		using (MemoryStream m = new MemoryStream ()) {
			ProtoArray<Block> protoArray = ProtoArrayFix.ToProtoArray<Block> (blocks);
			Serializer.Serialize (m, protoArray);

			byte[] bytesToSend = m.ToArray();

			int sequence = 0;
			while (bytesToSend.Length > 0) {
				byte[] singleMessage = bytesToSend.Take (maxMessageSize).ToArray ();
				TargetRpcGetChunk (identity.connectionToClient, worldPos, singleMessage, sequence, bytesToSend.Length > maxMessageSize);
				bytesToSend = bytesToSend.Skip (maxMessageSize).ToArray ();
				sequence++;
			}
		}
	}

	[TargetRpc]
	public void TargetRpcGetChunk(NetworkConnection target, WorldPos worldPos, byte[] bytes, int sequence, bool more) {
		//Debug.Log ("Recieved " + bytes.Length + " bytes.");
		if (sequence == 0) {
			lastSequence = -1;
			_bytes = bytes;
		} else {
			if (sequence != lastSequence + 1) {
				Debug.LogError ("Messages out of sequence! Sequence: " + sequence + " LastSequence: " + lastSequence + " isServer: " + isServer);
			}

			int originalLength = _bytes.Length;
			Array.Resize<byte> (ref _bytes, originalLength + bytes.Length);
			Array.Copy (bytes, 0, _bytes, originalLength, bytes.Length);
		}

		lastSequence = sequence;

		if (!more) {
			using (MemoryStream m = new MemoryStream (_bytes)) {
				ProtoArray<Block> protoArray = Serializer.Deserialize<ProtoArray<Block>> (m);
				Block[,,] blocks = (Block[,,])protoArray.ToArray<Block> ();
				World.Instance.chunkQueue.Enqueue (blocks);
			}
		}
	}
}
