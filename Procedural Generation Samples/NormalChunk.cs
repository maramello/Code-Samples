using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NormalChunk : MonoBehaviour {

	[Space]
	[Header("Prefab Settings")]
	public GameObject normalChunkPrefab;
	public GameObject woodenFloorPrefab;
	public GameObject rotatingKnifePrefab;
	public GameObject forkPrefab;
	public GameObject bowlPrefab;
	public GameObject cookieJarPrefab;
	public GameObject cookieJarLidPrefab;
	public GameObject knifeHolderPrefab;
	public GameObject panAndStovePrefab;

	public bool justSpawnedNormalChunk;
	public bool justSpawnedPanAndStove;
	public Generator generator;

	void Update() {
		
	}

	public void spawn() {
	
		if (justSpawnedNormalChunk) {
		
			generator.spawnX += 0.3f;
			generator.spawnY += 0.4f;

		}

		GameObject parent;

		//Lower ground
		parent = Instantiate (normalChunkPrefab, new Vector3 (generator.spawnX, generator.spawnY), Quaternion.identity);

		//Upper ground for fork boundary
		GameObject upper = Instantiate (woodenFloorPrefab, new Vector3 (generator.spawnX , generator.spawnY+10), Quaternion.identity);

		upper.transform.parent = parent.transform;
		//upper.transform.SetParent (parent.transform, false);

		//REMOVED HERE
		randomizePrefabSpawn (parent);

		justSpawnedNormalChunk = true;

		generator.spawnX += woodenFloorPrefab.GetComponent<Renderer>().bounds.size.x;
	}

	public void randomizePrefabSpawn(GameObject parentOb) {
		float x = generator.spawnX - (woodenFloorPrefab.GetComponent<Renderer>().bounds.size.x / 4f);
		float x2 = generator.spawnX + (woodenFloorPrefab.GetComponent<Renderer>().bounds.size.x / 4f);

		switch (Random.Range(0, 6)) {

		case 0:  case 1:
			instantiateKillingPrefab (x , generator.spawnY, parentOb);
			instantiateObstaclePrefab (x2, generator.spawnY, parentOb);
			break;
		case 2: case 3: 
			instantiateObstaclePrefab (x, generator.spawnY, parentOb);
			instantiateKillingPrefab (x2, generator.spawnY, parentOb);
			break;

		case 4:
			instantiateKillingPrefab (x , generator.spawnY, parentOb);
			instantiateKillingPrefab (x2 , generator.spawnY, parentOb);
			break;

		case 5: 
			instantiateObstaclePrefab (x, generator.spawnY, parentOb);
			instantiateObstaclePrefab (x2, generator.spawnY, parentOb);
			break;

		}
	}

	public void instantiateKillingPrefab(float x, float y, GameObject parentOb) {

		GameObject[] prefabs = {rotatingKnifePrefab, forkPrefab, knifeHolderPrefab, panAndStovePrefab};
		GameObject currPrefab;

		//Random for killing prefab
		int random = Random.Range (0, prefabs.Length);
		if (random != 3)
			justSpawnedPanAndStove = false;
		currPrefab = prefabs [random];

		currPrefab = prefabs [random];

		//GAMEOBJECT TO MAKE CHILD
		GameObject child;

		switch (random) {
		case 0:
			//Rotating Knife
			child = Instantiate (rotatingKnifePrefab, new Vector3 (x, y + 2.5f), Quaternion.identity);
			break;

		case 1: 
			child = Instantiate (forkPrefab, new Vector3 (x, y + Random.Range (2, 4)), Quaternion.identity);
			break;

		case 2: 
			//knife holder prefab
			child = Instantiate (knifeHolderPrefab, new Vector3 (x, y + Random.Range (7, 9)), Quaternion.identity);
			child.transform.Rotate (0, 0, -90);
			child.GetComponent<KnifeHolder> ().interval = Random.Range (3, 6);
			break;

		case 3: 
			//pan and stove prefab
			if (justSpawnedPanAndStove) {
				justSpawnedPanAndStove = false;
				return;
			}
			child = Instantiate (panAndStovePrefab, new Vector3 (x + currPrefab.GetComponentInChildren<Renderer> ().bounds.size.x, y + currPrefab.GetComponentInChildren<Renderer> ().bounds.size.y + 0.8f), Quaternion.identity);
			justSpawnedPanAndStove = true;
			break;

		default :
			child = Instantiate (currPrefab, new Vector3 (x, y + currPrefab.GetComponent<Renderer>().bounds.size.y/2), Quaternion.identity);
			break;
		}

		child.transform.parent = parentOb.transform;
	//	child.transform.SetParent (parentOb.transform, false);
	}

	public void instantiateObstaclePrefab(float x, float y, GameObject parentOb) {

		GameObject[] prefabs = {bowlPrefab, cookieJarPrefab, cookieJarLidPrefab};

		GameObject currPrefab;

		//Random for killing prefab
		int random = Random.Range (0, prefabs.Length);

		currPrefab = prefabs [random];


		currPrefab = prefabs [random];

		GameObject child;

		switch (random) {
		case 2: 
			child = Instantiate (cookieJarLidPrefab, new Vector3 (x, y + 0.7f), Quaternion.identity);
			break;

		default :
			child = Instantiate (currPrefab, new Vector3 (x, y + currPrefab.GetComponent<Renderer>().bounds.size.y/2), Quaternion.identity);
			break;
		}

		child.transform.parent = parentOb.transform;
	//	child.transform.SetParent (parentOb.transform, false);
	}
}
