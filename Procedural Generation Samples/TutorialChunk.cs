using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TutorialChunk : MonoBehaviour {

	public GameObject tutorialChunkPrefab;
	public GameObject woodenFloorPrefab;

	public Generator generator;

	void Update() {

	}

	public void spawn() {

		//Lower ground
		Instantiate (tutorialChunkPrefab, new Vector3 (generator.spawnX, generator.spawnY), Quaternion.identity);

		//Upper ground for fork boundary
		Instantiate (woodenFloorPrefab, new Vector3 (generator.spawnX , generator.spawnY+10), Quaternion.identity);

		//Upper ground for second area
		Instantiate (woodenFloorPrefab, new Vector3 (generator.spawnX + woodenFloorPrefab.GetComponent<Renderer>().bounds.size.x , generator.spawnY+10), Quaternion.identity);

		generator.spawnX += (woodenFloorPrefab.GetComponent<Renderer>().bounds.size.x * 2);
	}
}
