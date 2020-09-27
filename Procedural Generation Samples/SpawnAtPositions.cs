using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnAtPositions : MonoBehaviour {

	public GameObject[] spawnableObjects;

	public Transform[] spawnpoints;

	// Use this for initialization
	void Start () {
		for (int i = 0; i < spawnpoints.Length; i++) {
			
			int random = Random.Range (0, spawnableObjects.Length);

			GameObject instance;

			instance = Instantiate (spawnableObjects [random], spawnpoints [i].transform.position, Quaternion.identity);
			
			instance.transform.parent = gameObject.transform;
		//	instance.transform.SetParent (gameObject.transform, true);
		}
	}

}
