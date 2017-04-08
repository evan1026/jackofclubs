﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TerrainHelper {

	public static WorldPos GetBlockPos (Vector3 pos) {
		WorldPos blockPos = new WorldPos (
			                    Mathf.RoundToInt (pos.x),
			                    Mathf.RoundToInt (pos.y),
			                    Mathf.RoundToInt (pos.z)
		                    );

		return blockPos;
	}

	public static WorldPos GetBlockPos (RaycastHit hit, bool adjacent = false) {
		Vector3 pos = new Vector3 (
			              MoveWithinBlock (hit.point.x, hit.normal.x, adjacent),
			              MoveWithinBlock (hit.point.y, hit.normal.y, adjacent),
			              MoveWithinBlock (hit.point.z, hit.normal.z, adjacent)
		              );

		return GetBlockPos (pos);
	}

	static float MoveWithinBlock (float pos, float norm, bool adjacent = false) {
		if (Mathf.Abs(pos - (int)pos) - 0.5f < 0.01) { //We are approximately in between blocks
			if (adjacent) {
				pos += (norm / 2);
			} else {
				pos -= (norm / 2);
			}
		}

		return (float)pos;
	}

	public static bool SetBlock (RaycastHit hit, Block block, bool adjacent = false) {
		Chunk chunk = hit.collider.GetComponent<Chunk> ();
		if (chunk == null) {
			return false;
		}

		WorldPos pos = GetBlockPos (hit, adjacent);

		chunk.world.SetBlock (pos.x, pos.y, pos.z, block, true);

		return true;
	}

	public static Block GetBlock (RaycastHit hit, bool adjacent = false) {
		Chunk chunk = hit.collider.GetComponent<Chunk> ();
		if (chunk == null)
			return null;

		WorldPos pos = GetBlockPos (hit, adjacent);

		Block block = chunk.world.GetBlock (pos.x, pos.y, pos.z);

		return block;
	}
}
