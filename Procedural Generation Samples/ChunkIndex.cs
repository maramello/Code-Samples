using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChunkIndex : MonoBehaviour {

	public int chunkIndex;
	public static int currChunkIndex;

	// Use this for initialization
	void Start () {
		chunkIndex = currChunkIndex;
		currChunkIndex++;
	}
	
	// Update is called once per frame
	void Update () {
		if (chunkIndex <= (Generator.numberOfChunks - 12))
		if (gameObject != null) {
			print ("destroying chunk " + chunkIndex);
			Destroy (gameObject);
		}
	}
}
