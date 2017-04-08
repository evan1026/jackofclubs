using UnityEngine;
using System.Collections;
using System.IO;
using System;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization;

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

		IFormatter formatter = new BinaryFormatter();
		Stream stream = new FileStream(saveFile, FileMode.Create, FileAccess.Write, FileShare.None);
		formatter.Serialize(stream, chunk.blocks);
		stream.Close();
	}

	public static bool loadChunk(Chunk chunk) {
		string saveFile = getSaveLocation(chunk.world.worldName);
		saveFile += getChunkFileName(chunk.pos);

		if (!File.Exists(saveFile))
			return false;

		try {
			IFormatter formatter = new BinaryFormatter();
			FileStream stream = new FileStream(saveFile, FileMode.Open);

			chunk.blocks = (Block[,,]) formatter.Deserialize(stream);
			stream.Close();

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

		IFormatter formatter = new BinaryFormatter();
		Stream stream = new FileStream(saveFile, FileMode.Create, FileAccess.Write, FileShare.None);
		formatter.Serialize (stream, w.meta);
		stream.Close();
	}

	public static bool loadMeta(World w) {
		string saveFile = getSaveLocation(w.worldName);
		saveFile += getMetaFileName(w);

		if (!File.Exists(saveFile))
			return false;

		try {
			IFormatter formatter = new BinaryFormatter();
			FileStream stream = new FileStream(saveFile, FileMode.Open);

			w.meta = (WorldMetaData) formatter.Deserialize(stream);
			stream.Close();
			return true;
		} catch (Exception e) {
			Debug.LogException (e);
			return false;
		}
	}
}