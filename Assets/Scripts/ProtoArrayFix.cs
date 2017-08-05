// https://stackoverflow.com/questions/22023405/protobuf-with-multidimensional-array

using System;
using ProtoBuf;

public static class ProtoArrayFix
{

	#region Multidimensional array handling

	public static ProtoArray<T> ToProtoArray<T>(this Array array)
	{
		// Copy dimensions (to be used for reconstruction).
		var dims = new int[array.Rank];
		for (int i = 0; i < array.Rank; i++) dims[i] = array.GetLength(i);
		// Copy the underlying data.
		var data = new T[array.Length];
		var k = 0;
		array.MultiLoop(indices => data[k++] = (T) array.GetValue(indices));

		return new ProtoArray<T> {Dimensions = dims, Data = data};
	}

	public static Array ToArray<T>(this ProtoArray<T> protoArray)
	{
		// Initialize array dynamically.
		var result = Array.CreateInstance(typeof(T), protoArray.Dimensions);
		// Copy the underlying data.
		var k = 0;
		result.MultiLoop(indices => result.SetValue(protoArray.Data[k++], indices));

		return result;
	}

	#endregion

	#region Array extensions

	public static void MultiLoop(this Array array, Action<int[]> action)
	{
		array.RecursiveLoop(0, new int[array.Rank], action);
	}

	private static void RecursiveLoop(this Array array, int level, int[] indices, Action<int[]> action)
	{
		if (level == array.Rank)
		{
			action(indices);
		}
		else
		{
			for (indices[level] = 0; indices[level] < array.GetLength(level); indices[level]++)
			{
				RecursiveLoop(array, level + 1, indices, action);
			}
		}
	}

	#endregion
}

[ProtoContract]
public class ProtoArray<T>
{
	[ProtoMember(1)]
	public int[] Dimensions { get; set; }
	[ProtoMember(2)]
	public T[] Data { get; set; }
}