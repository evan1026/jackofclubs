﻿using UnityEngine;
using System.Collections;
using System;
using ProtoBuf;

[ProtoContract]
[ProtoInclude(1, typeof(BlockAir))]
public class Block {
	public enum Direction {
		north,
		east,
		south,
		west,
		up,
		down
	};

	public struct Tile { public int x; public int y;};

	const float tileSize = 0.5f;

	[ProtoMember(2)]
	public SerializableColor color;

	//Base block constructor
	public Block (Color _color) {
		color = new SerializableColor(_color);
	}

	public Block () : this(new Color32(0,0,0,0)) {}

	public virtual MeshData Blockdata (Chunk chunk, int x, int y, int z, MeshData meshData) {
		meshData.useRenderDataForCol = true;

		if (!chunk.GetBlock (x, y + 1, z).IsSolid (Direction.down)) {
			meshData = FaceDataUp (chunk, x, y, z, meshData);
		}

		if (!chunk.GetBlock (x, y - 1, z).IsSolid (Direction.up)) {
			meshData = FaceDataDown (chunk, x, y, z, meshData);
		}

		if (!chunk.GetBlock (x, y, z + 1).IsSolid (Direction.south)) {
			meshData = FaceDataNorth (chunk, x, y, z, meshData);
		}

		if (!chunk.GetBlock (x, y, z - 1).IsSolid (Direction.north)) {
			meshData = FaceDataSouth (chunk, x, y, z, meshData);
		}

		if (!chunk.GetBlock (x + 1, y, z).IsSolid (Direction.west)) {
			meshData = FaceDataEast (chunk, x, y, z, meshData);
		}

		if (!chunk.GetBlock (x - 1, y, z).IsSolid (Direction.east)) {
			meshData = FaceDataWest (chunk, x, y, z, meshData);
		}

		return meshData;

	}

	protected virtual MeshData FaceDataUp (Chunk chunk, int x, int y, int z, MeshData meshData) {
		meshData.AddVertex(new Vector3(x - 0.5f, y + 0.5f, z + 0.5f));
		meshData.AddVertex(new Vector3(x + 0.5f, y + 0.5f, z + 0.5f));
		meshData.AddVertex(new Vector3(x + 0.5f, y + 0.5f, z - 0.5f));
		meshData.AddVertex(new Vector3(x - 0.5f, y + 0.5f, z - 0.5f));
		meshData.AddQuadTriangles();
		meshData.uv.AddRange(FaceUVs(Direction.up));
		meshData.colors.AddRange (FaceColors ());
		return meshData;
	}

	protected virtual MeshData FaceDataDown (Chunk chunk, int x, int y, int z, MeshData meshData) {
		meshData.AddVertex(new Vector3(x - 0.5f, y - 0.5f, z - 0.5f));
		meshData.AddVertex(new Vector3(x + 0.5f, y - 0.5f, z - 0.5f));
		meshData.AddVertex(new Vector3(x + 0.5f, y - 0.5f, z + 0.5f));
		meshData.AddVertex(new Vector3(x - 0.5f, y - 0.5f, z + 0.5f));
		meshData.AddQuadTriangles();
		meshData.uv.AddRange(FaceUVs(Direction.down));
		meshData.colors.AddRange (FaceColors ());
		return meshData;
	}

	protected virtual MeshData FaceDataNorth (Chunk chunk, int x, int y, int z, MeshData meshData) {
		meshData.AddVertex(new Vector3(x + 0.5f, y - 0.5f, z + 0.5f));
		meshData.AddVertex(new Vector3(x + 0.5f, y + 0.5f, z + 0.5f));
		meshData.AddVertex(new Vector3(x - 0.5f, y + 0.5f, z + 0.5f));
		meshData.AddVertex(new Vector3(x - 0.5f, y - 0.5f, z + 0.5f));
		meshData.AddQuadTriangles();
		meshData.uv.AddRange(FaceUVs(Direction.north));
		meshData.colors.AddRange (FaceColors ());
		return meshData;
	}

	protected virtual MeshData FaceDataEast (Chunk chunk, int x, int y, int z, MeshData meshData) {
		meshData.AddVertex(new Vector3(x + 0.5f, y - 0.5f, z - 0.5f));
		meshData.AddVertex(new Vector3(x + 0.5f, y + 0.5f, z - 0.5f));
		meshData.AddVertex(new Vector3(x + 0.5f, y + 0.5f, z + 0.5f));
		meshData.AddVertex(new Vector3(x + 0.5f, y - 0.5f, z + 0.5f));
		meshData.AddQuadTriangles();
		meshData.uv.AddRange(FaceUVs(Direction.east));
		meshData.colors.AddRange (FaceColors ());
		return meshData;
	}

	protected virtual MeshData FaceDataSouth (Chunk chunk, int x, int y, int z, MeshData meshData) {
		meshData.AddVertex(new Vector3(x - 0.5f, y - 0.5f, z - 0.5f));
		meshData.AddVertex(new Vector3(x - 0.5f, y + 0.5f, z - 0.5f));
		meshData.AddVertex(new Vector3(x + 0.5f, y + 0.5f, z - 0.5f));
		meshData.AddVertex(new Vector3(x + 0.5f, y - 0.5f, z - 0.5f));
		meshData.AddQuadTriangles();
		meshData.uv.AddRange(FaceUVs(Direction.south));
		meshData.colors.AddRange (FaceColors ());
		return meshData;
	}

	protected virtual MeshData FaceDataWest (Chunk chunk, int x, int y, int z, MeshData meshData) {
		meshData.AddVertex(new Vector3(x - 0.5f, y - 0.5f, z + 0.5f));
		meshData.AddVertex(new Vector3(x - 0.5f, y + 0.5f, z + 0.5f));
		meshData.AddVertex(new Vector3(x - 0.5f, y + 0.5f, z - 0.5f));
		meshData.AddVertex(new Vector3(x - 0.5f, y - 0.5f, z - 0.5f));
		meshData.AddQuadTriangles();
		meshData.uv.AddRange(FaceUVs(Direction.west));
		meshData.colors.AddRange (FaceColors ());
		return meshData;
	}

	public virtual bool IsSolid (Direction direction) {
		switch (direction) {
		case Direction.north:
			return true;
		case Direction.east:
			return true;
		case Direction.south:
			return true;
		case Direction.west:
			return true;
		case Direction.up:
			return true;
		case Direction.down:
			return true;
		}
		return false;
	}

	public virtual Tile TexturePosition(Direction direction) {
		Tile tile = new Tile();
		tile.x = 0;
		tile.y = 0;
		return tile;
	}

	public virtual Vector2[] FaceUVs(Direction direction) {
		Vector2[] UVs = new Vector2[4];
		Tile tilePos = TexturePosition(direction);
		UVs[0] = new Vector2(tileSize * tilePos.x + tileSize,
			tileSize * tilePos.y);
		UVs[1] = new Vector2(tileSize * tilePos.x + tileSize,
			tileSize * tilePos.y + tileSize);
		UVs[2] = new Vector2(tileSize * tilePos.x,
			tileSize * tilePos.y + tileSize);
		UVs[3] = new Vector2(tileSize * tilePos.x,
			tileSize * tilePos.y);
		return UVs;
	}

	public virtual Color[] FaceColors() {
		Color[] colors = new Color[4];
		colors [0] = color.getColor();
		colors [1] = color.getColor();
		colors [2] = color.getColor();
		colors [3] = color.getColor();
		return colors;
	}
}
