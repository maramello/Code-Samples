using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Generator : MonoBehaviour {

	[Header("Generation Settings")]
	public float startPositionX;
	public float startFloorY;
	public int chunks;



	[Space]
	[Header("Prefab Settings")]
	public GameObject woodenFloorPrefab;

	[Space]
	[Header("Chunks")]
	public NormalChunk normalChunk;
	public WaterChunk waterChunk;
	public MovingSpoonChunk movingSpoonChunk;
	public TrapdoorChunk trapdoorChunk;
	public DownNormalChunk downNormalChunk;
	public CoinChunk coinChunk;
	public TutorialChunk tutorialChunk;


	public float spawnX;
	public float spawnY;

	[Space]
	public SpriteRenderer backgroundRenderer;

	[Space]
	[Header("Background Colors")]
	public Color[] backgrounds;

	public static bool shouldGenerateNextChunk;
	public static int currScore;
	public static int numberOfChunks = 0;

	// Use this for initialization
	void Start () {
		shouldGenerateNextChunk = false;
		currScore = 0;
		spawnX = startPositionX;
		spawnY = startFloorY;
		Generator.numberOfChunks = 0;

		//	PlayerPrefs.SetString ("firstTime", "yes");

		if (PlayerPrefs.GetString ("Tutorial") == "off") {
			print ("tutorial is off ffs");
		}

		else if (PlayerPrefs.GetString ("Tutorial") == "on") {
			//Tutorial
			print("starting tutorial");
			generateTutorialChunk();
			PlayerPrefs.SetString ("Tutorial", "off");
		}

		for (int i = 0; i < chunks; i++) {
			Generate ();
		}
	}

	void Update() {
		if (Generator.shouldGenerateNextChunk) {
			Generate ();
			Generator.shouldGenerateNextChunk = false;
		}
		if (currScore < 500)
			backgroundRenderer.color = backgrounds [currScore / 100];
		else
			backgroundRenderer.color = backgrounds [5];
	}

	public void generateTutorialChunk() {
		tutorialChunk.spawn ();
	}

	public void Generate() {

		int randomChunk = Random.Range (0, 7);
		//Lower ground
		switch (randomChunk) {
		case 0: case 1:
			normalChunk.spawn ();
			break;

		case 2: 
			normalChunk.justSpawnedNormalChunk = false;
			downNormalChunk.spawn ();
			break;

		case 3: 
			normalChunk.justSpawnedNormalChunk = false;
			waterChunk.spawn ();
			break;

		case 4: 
			normalChunk.justSpawnedNormalChunk = false;
			movingSpoonChunk.spawn ();
			break;

		case 5:
			normalChunk.justSpawnedNormalChunk = false;
			trapdoorChunk.spawn ();
			break;

		case 6:
			normalChunk.justSpawnedNormalChunk = false;
			coinChunk.spawn ();
			break;

		default : 
			normalChunk.spawn ();
			break;

		} //end of switch

		Generator.numberOfChunks = Generator.numberOfChunks + 1;

	}

}
