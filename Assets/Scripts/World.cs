using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class World : MonoBehaviour {

	public Dictionary<WorldPos, Chunk> chunks = new Dictionary<WorldPos, Chunk> ();
	public GameObject chunkPrefab;
	public GameObject wallParent;
	public GameObject player;
	public GameObject loadingScreen;
	public GameObject savingScreen;

	private static string wallName = "Wall";

	public string worldName;

	public WorldMetaData meta;

	IEnumerator Start () {
		Time.timeScale = 0;

		LoadingBarScript loadingBar = loadingScreen.GetComponentInChildren<LoadingBarScript> ();

		if (worldName.Equals ("")) {
			Debug.LogError ("World name cannot be empty!");
			Application.Quit ();
		}

		if (!Serialization.loadMeta (this)) {
			Debug.Log ("Metadata file not found! Using default values.");

			meta = new WorldMetaData ();
			meta.dimension = new SerializableVector3 (new Vector3 (8, 8, 8));
		}

		if (meta.dimension.x < 1 || meta.dimension.y < 1 || meta.dimension.z < 1) {
			Debug.LogError ("Dimensions must all be at least 1!");
			Application.Quit ();
		}

		int xStart = -Mathf.FloorToInt(meta.dimension.x / 2);
		int xEnd = Mathf.CeilToInt(meta.dimension.x / 2);
		int yStart = 0;
		int yEnd = Mathf.FloorToInt(meta.dimension.y);
		int zStart = -Mathf.FloorToInt(meta.dimension.z / 2);
		int zEnd = Mathf.CeilToInt(meta.dimension.z / 2);

		int chunksTotal = (int)(meta.dimension.x * meta.dimension.y * meta.dimension.z);
		int chunksSoFar = 0;

		for (int x = xStart; x < xEnd; x++) {
			for (int y = yStart; y < yEnd; y++) {
				for (int z = zStart; z < zEnd; z++) {
					CreateChunk (x * Chunk.chunkSize, y * Chunk.chunkSize, z * Chunk.chunkSize);
					chunksSoFar++;
					loadingBar.setPercent (((float)chunksSoFar) / chunksTotal);
					yield return null;
				}
			}
		}

		updateAllChunks ();

		generateWalls (xStart, xEnd, yStart, zStart, zEnd);

		loadingScreen.SetActive (false);

		Time.timeScale = 1;
	}

	void Update() {
		if (Input.GetKeyDown ("q")) {
			StartCoroutine ("OnQuit");
		}
	}

	IEnumerator OnQuit() {
		savingScreen.SetActive (true);
		Time.timeScale = 0;

		int chunksTotal = (int)(meta.dimension.x * meta.dimension.y * meta.dimension.z);
		int chunksSoFar = 0;

		LoadingBarScript loadingBar = savingScreen.GetComponentInChildren<LoadingBarScript> ();

		WorldPos[] keys = new WorldPos[chunks.Keys.Count];
		chunks.Keys.CopyTo (keys, 0);

		foreach (WorldPos chunkPos in keys) {
			DestroyChunk(chunkPos.x, chunkPos.y, chunkPos.z);
			chunksSoFar++;
			loadingBar.setPercent (((float)chunksSoFar) / chunksTotal);
			yield return null;
		}

		Serialization.saveMeta (this);

		Debug.Log ("Quitting!");
		Application.Quit ();
	}

	private static float wallDepth = 0.1f;
	private void generateWalls(int xStart, int xEnd, int yStart, int zStart, int zEnd) {
		float zLength = (float)((zEnd - zStart) * Chunk.chunkSize);
		float xLength = (float)((xEnd - xStart) * Chunk.chunkSize);
		float yLength = 1000f;

		GameObject minusX = GameObject.CreatePrimitive (PrimitiveType.Cube);
		minusX.name = wallName;
		minusX.transform.parent = wallParent.transform;
		minusX.transform.position = new Vector3 ((float)(xStart*Chunk.chunkSize - 0.5 - wallDepth / 2), 0f, -0.5f);
		minusX.transform.localScale = new Vector3 (wallDepth, yLength, zLength);

		GameObject plusX = GameObject.CreatePrimitive (PrimitiveType.Cube);
		plusX.name = wallName;
		plusX.transform.parent = wallParent.transform;
		plusX.transform.position = new Vector3 ((float)(xEnd*Chunk.chunkSize - 0.5 + wallDepth / 2), 0f, -0.5f);
		plusX.transform.localScale = new Vector3 (wallDepth, yLength, zLength);

		GameObject minusY = GameObject.CreatePrimitive (PrimitiveType.Cube);
		minusY.name = wallName;
		plusX.transform.parent = wallParent.transform;
		minusY.transform.position = new Vector3 (-0.5f, (float)(yStart*Chunk.chunkSize - 0.5 - wallDepth / 2), -0.5f);
		minusY.transform.localScale = new Vector3 (xLength, wallDepth, zLength);

		GameObject minusZ = GameObject.CreatePrimitive (PrimitiveType.Cube);
		minusZ.name = wallName;
		minusZ.transform.parent = wallParent.transform;
		minusZ.transform.position = new Vector3 (-0.5f, 0f, (float)(zStart*Chunk.chunkSize - 0.5 - wallDepth / 2));
		minusZ.transform.localScale = new Vector3 (xLength, yLength, wallDepth);

		GameObject plusZ = GameObject.CreatePrimitive (PrimitiveType.Cube);
		plusZ.name = wallName;
		plusZ.transform.parent = wallParent.transform;
		plusZ.transform.position = new Vector3 (-0.5f, 0f, (float)(zEnd*Chunk.chunkSize - 0.5 + wallDepth / 2));
		plusZ.transform.localScale = new Vector3 (xLength, yLength, wallDepth);

		// Make dem walls invisible
		Destroy (minusX.GetComponent<MeshRenderer> ());
		Destroy (plusX.GetComponent<MeshRenderer> ());
		Destroy (minusY.GetComponent<MeshRenderer> ());
		Destroy (minusZ.GetComponent<MeshRenderer> ());
		Destroy (plusZ.GetComponent<MeshRenderer> ());
	}

	public void CreateChunk (int x, int y, int z) {
		WorldPos worldPos = new WorldPos (x, y, z);

		//Instantiate the chunk at the coordinates using the chunk prefab
		GameObject newChunkObject = Instantiate (chunkPrefab, new Vector3 (x, y, z), Quaternion.identity) as GameObject;
		newChunkObject.transform.parent = gameObject.transform;

		Chunk newChunk = newChunkObject.GetComponent<Chunk> ();

		newChunk.pos = worldPos;
		newChunk.world = this;

		//Add it to the chunks dictionary with the position as the key
		chunks.Add (worldPos, newChunk);

		bool loaded = Serialization.loadChunk (newChunk);

		newChunk.update = false;

		if (loaded) {
			return;
		}
		
		for (int xi = 0; xi < Chunk.chunkSize; ++xi) {
			for (int yi = 0; yi < Chunk.chunkSize; ++yi) {
				for (int zi = 0; zi < Chunk.chunkSize; ++zi) {
					if (y + yi <= 40) {
						Color32 color;
						color.r = (byte)(255f / Chunk.chunkSize * xi);
						color.g = (byte)(255f / Chunk.chunkSize * yi);
						color.b = (byte)(255f / Chunk.chunkSize * zi);
						color.a = 255;
						SetBlock (x + xi, y + yi, z + zi, new Block (color), false);
					} else {
						SetBlock (x + xi, y + yi, z + zi, new BlockAir (), false);
					}
				}
			}
		}

		newChunk.modifiedSinceLastSave = true;
	}

	public void DestroyChunk (int x, int y, int z) {
		Chunk chunk = null;
		if (chunks.TryGetValue (new WorldPos (x, y, z), out chunk)) {
			Serialization.saveChunk(chunk);
			Object.Destroy (chunk.gameObject);
			chunks.Remove (new WorldPos (x, y, z));
		}
	}

	public Chunk GetChunk (int x, int y, int z) {
		WorldPos pos = new WorldPos ();
		float multiple = Chunk.chunkSize;
		pos.x = Mathf.FloorToInt (x / multiple) * Chunk.chunkSize;
		pos.y = Mathf.FloorToInt (y / multiple) * Chunk.chunkSize;
		pos.z = Mathf.FloorToInt (z / multiple) * Chunk.chunkSize;
		Chunk containerChunk = null;
		chunks.TryGetValue (pos, out containerChunk);

		return containerChunk;
	}

	public Block GetBlock (int x, int y, int z) {
		Chunk containerChunk = GetChunk (x, y, z);
		if (containerChunk != null) {
			Block block = containerChunk.GetBlock (x - containerChunk.pos.x, y - containerChunk.pos.y, z - containerChunk.pos.z);
			return block;
		} else {
			return new BlockAir ();
		}
	}

	public void SetBlock (int x, int y, int z, Block block, bool update) {
		Chunk chunk = GetChunk (x, y, z);

		Collider[] stuffInTheWay = Physics.OverlapBox (new Vector3 (x, y, z), new Vector3 (0.5f, 0.5f, 0.5f));
		if (stuffInTheWay.Length > 0 && !(block is BlockAir)) {
			foreach (Collider c in stuffInTheWay) {
				if (c.GetComponent<Chunk> () == null && !c.name.Equals(wallName)) {
					Debug.Log ("Collided with: " + c.name + ". Aborting block placement.");
					return;
				}
			}
		}

		if (chunk != null) {
			chunk.SetBlock (x - chunk.pos.x, y - chunk.pos.y, z - chunk.pos.z, block);

			if (update) {
				chunk.update = true;

				UpdateIfEqual (x - chunk.pos.x, 0, new WorldPos (x - 1, y, z));
				UpdateIfEqual (x - chunk.pos.x, Chunk.chunkSize - 1, new WorldPos (x + 1, y, z));
				UpdateIfEqual (y - chunk.pos.y, 0, new WorldPos (x, y - 1, z));
				UpdateIfEqual (y - chunk.pos.y, Chunk.chunkSize - 1, new WorldPos (x, y + 1, z));
				UpdateIfEqual (z - chunk.pos.z, 0, new WorldPos (x, y, z - 1));
				UpdateIfEqual (z - chunk.pos.z, Chunk.chunkSize - 1, new WorldPos (x, y, z + 1));
			}
		}
	}

	void updateAllChunks() {
		foreach (Chunk c in chunks.Values) {
			c.update = true;
		}
	}

	void UpdateIfEqual (int value1, int value2, WorldPos pos) {
		if (value1 == value2) {
			Chunk chunk = GetChunk (pos.x, pos.y, pos.z);
			if (chunk != null)
				chunk.update = true;
		}
	}
}
