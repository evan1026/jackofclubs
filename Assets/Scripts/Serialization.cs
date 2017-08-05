using UnityEngine;
using System.Collections;
using System.IO;
using System;
using ProtoBuf;

public static class Serialization {

	public static string saveFolderName = "saves";

	public static string getSaveLocation(string worldName) {
		string saveLocation = saveFolderName + "/" + worldName + "/";

		if (!Directory.Exists(saveLocation)) {
			Directory.CreateDirectory(saveLocation);
		}

		return saveLocation;
	}

	public static string getChunkFileName(WorldPos chunkLocation) {
		string fileName = chunkLocation.x + "," + chunkLocation.y+ "," + chunkLocation.z +".bin";
		return fileName;
	}

	public static string getMetaFileName(World w) {
		return w.worldName + ".meta";
	}

	public static void saveChunk(Chunk chunk) {
		if (!chunk.modifiedSinceLastSave)
			return;
		
		string saveFile = getSaveLocation(chunk.world.worldName);
		saveFile += getChunkFileName(chunk.pos);

		ProtoArray<Block> protoArray = ProtoArrayFix.ToProtoArray<Block> (chunk.blocks);

		Stream stream = new FileStream(saveFile, FileMode.Create, FileAccess.Write, FileShare.None);
		Serializer.Serialize(stream, protoArray);
		stream.Close();
	}

	public static bool loadChunk(Chunk chunk) {
		string saveFile = getSaveLocation(chunk.world.worldName);
		saveFile += getChunkFileName(chunk.pos);

		if (!File.Exists(saveFile))
			return false;

		try {
			FileStream stream = new FileStream(saveFile, FileMode.Open);

			ProtoArray<Block> protoArray = Serializer.Deserialize<ProtoArray<Block>>(stream);
			stream.Close();

			chunk.blocks = (Block[,,])protoArray.ToArray<Block>();

			chunk.modifiedSinceLastSave = false;

			return true;
		} catch (Exception e) {
			Debug.LogException (e);
			return false;
		}
	}

	public static void saveMeta(World w) {
		string saveFile = getSaveLocation(w.worldName);
		saveFile += getMetaFileName(w);

		Stream stream = new FileStream(saveFile, FileMode.Create, FileAccess.Write, FileShare.None);
		Serializer.Serialize(stream, w.meta);
		stream.Close();
	}

	public static bool loadMeta(World w) {
		string saveFile = getSaveLocation(w.worldName);
		saveFile += getMetaFileName(w);

		if (!File.Exists(saveFile))
			return false;

		try {
			FileStream stream = new FileStream(saveFile, FileMode.Open);

			w.meta = (WorldMetaData) Serializer.Deserialize<WorldMetaData>(stream);
			stream.Close();
			return true;
		} catch (Exception e) {
			Debug.LogException (e);
			return false;
		}
	}
}