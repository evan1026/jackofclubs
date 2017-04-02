using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class World : MonoBehaviour {

	public Dictionary<WorldPos, Chunk> chunks = new Dictionary<WorldPos, Chunk> ();
	public GameObject chunkPrefab;
	public GameObject wallParent;

	private static string wallName = "Wall";

	// Use this for initialization
	void Start () {
		int xStart = -4;
		int xEnd = 4;
		int yStart = 0;
		int yEnd = 8;
		int zStart = -4;
		int zEnd = 4;

		for (int x = xStart; x < xEnd; x++) {
			for (int y = yStart; y < yEnd; y++) {
				for (int z = zStart; z < zEnd; z++) {
					CreateChunk (x * Chunk.chunkSize, y * Chunk.chunkSize, z * Chunk.chunkSize);
				}
			}
		}

		generateWalls (xStart, xEnd, yStart, zStart, zEnd);
	}

	// Update is called once per frame
	void Update () {
		
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

		for (int xi = 0; xi < Chunk.chunkSize; ++xi) {
			for (int yi = 0; yi < Chunk.chunkSize; ++yi) {
				for (int zi = 0; zi < Chunk.chunkSize; ++zi) {
					if (y + yi <= 40) {
						Color32 color;
						color.r = (byte)(255f / Chunk.chunkSize * xi);
						color.g = (byte)(255f / Chunk.chunkSize * yi);
						color.b = (byte)(255f / Chunk.chunkSize * zi);
						color.a = 255;
						SetBlock (x + xi, y + yi, z + zi, new Block (color));
					} else {
						SetBlock (x + xi, y + yi, z + zi, new BlockAir ());
					}
				}
			}
		}
	}

	public void DestroyChunk (int x, int y, int z) {
		Chunk chunk = null;
		if (chunks.TryGetValue (new WorldPos (x, y, z), out chunk)) {
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

	public void SetBlock (int x, int y, int z, Block block) {
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
			chunk.update = true;

			UpdateIfEqual (x - chunk.pos.x, 0, new WorldPos (x - 1, y, z));
			UpdateIfEqual (x - chunk.pos.x, Chunk.chunkSize - 1, new WorldPos (x + 1, y, z));
			UpdateIfEqual (y - chunk.pos.y, 0, new WorldPos (x, y - 1, z));
			UpdateIfEqual (y - chunk.pos.y, Chunk.chunkSize - 1, new WorldPos (x, y + 1, z));
			UpdateIfEqual (z - chunk.pos.z, 0, new WorldPos (x, y, z - 1));
			UpdateIfEqual (z - chunk.pos.z, Chunk.chunkSize - 1, new WorldPos (x, y, z + 1));
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
