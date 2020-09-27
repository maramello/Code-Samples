using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine.EventSystems;
using UnityEngine.Advertisements;
using AppAdvisory.social;

public class MainMenu : MonoBehaviour {

	void Start() {

		//PlayerPrefs.SetInt ("Coin", 100);
		PlayerPrefs.SetString ("VideoAds", "On");

		currMusic ();
		if (!Advertisement.isInitialized)
		{
			print ("ad not initialized");
			if (Application.platform == RuntimePlatform.IPhonePlayer)
				Advertisement.Initialize ("1482921", true);
			else if (Application.platform == RuntimePlatform.Android)
				Advertisement.Initialize ("1482920", true);
			else
				print ("ads not supported");
		}

		if (PlayerPrefs.GetString("VideoAds") == "")
			PlayerPrefs.SetString ("VideoAds", "On");

		if (PlayerPrefs.GetString ("Tutorial") == "")
			PlayerPrefs.SetString ("Tutorial", "on");

		if (PlayerPrefs.GetString ("firstTime") == "" || PlayerPrefs.GetString ("firstTime") == "yes")
		{
			print ("first time playing this game");
			//	PlayerPrefs.SetString ("firstTime", "no");
			PlayerPrefs.SetString ("EquippedEgg", "eggbiggereyes");
			PlayerPrefs.SetString ("BrokenEgg", "cracked egg");
			PlayerPrefs.SetString ("Music", "On");
			PlayerPrefs.SetString ("Tutorial", "on");
			PlayerPrefs.SetString ("VideoAds", "On");
			GameObject.Find ("Music").GetComponent<MusicControl> ().updateState (true);

			//	SceneManager.LoadScene ("Tutorial", LoadSceneMode.Single);
			PlayerPrefs.SetString("firstTime", "no");
		}

		EventSystem.current.GetComponent<EventSystem>().SetSelectedGameObject(null);

	}

	public void onLeaderboardClick() {
		
			LeaderboardManager.ReportScore (PlayerPrefs.GetInt ("HighScore"));

			LeaderboardManager.ShowLeaderboardUI ();
	}

	public void onPlayClick() {

		GameObject.Find ("Music").GetComponent<DontDestroy> ().current = true;
		DontDestroy.firstTime = false;

		Game.isMultiplayer = false;
		

		//SceneManager.LoadScene ("Level3", LoadSceneMode.Single);
		SceneManager.LoadScene("ModeSelect", LoadSceneMode.Single);
	//	SceneManager.LoadScene("LevelInfinite", LoadSceneMode.Single);

		//Avoid picture problems
		if (PlayerPrefs.GetString("EquippedEgg") == "")
			PlayerPrefs.SetString ("EquippedEgg", "eggbiggereyes");

	}

	public void onLevelClick() {
		
		Button b = EventSystem.current.currentSelectedGameObject.GetComponent<Button>();

		string lvl = b.GetComponentInChildren<Text> ().text;

		if (lvl != "Infinite")
		ExitPortal.currentLevel = int.Parse (lvl);

		SceneManager.LoadScene ("Level" + lvl, LoadSceneMode.Single);
	}

	public void onMainMenuClick() {
		

		SceneManager.LoadScene ("MainMenu", LoadSceneMode.Single);
	}

	public void onMinigameClick() {
		
		GameObject.Find ("Music").GetComponent<DontDestroy> ().current = true;
		DontDestroy.firstTime = false;


		SceneManager.LoadScene ("Minigame", LoadSceneMode.Single);	
	}

	public void onShopClick() {
		GameObject.Find ("Music").GetComponent<DontDestroy> ().current = true;
		DontDestroy.firstTime = false;


		SceneManager.LoadScene ("Shop", LoadSceneMode.Single);
	}

	public void onOptionsClick() {
		GameObject.Find ("Music").GetComponent<DontDestroy> ().current = true;
		DontDestroy.firstTime = false;


		SceneManager.LoadScene ("Options", LoadSceneMode.Single);
	}

	public void onMultiplayerClick() {
		Game.isMultiplayer = true;

		SceneManager.LoadScene ("Lobby", LoadSceneMode.Single);	
	}

	public void onStopGameMultiplayerClick() {

		SceneManager.LoadScene ("MultiplayerMenu", LoadSceneMode.Additive);
	}

	public void onBackMultiplayerClick() {
		Destroy (GameObject.FindGameObjectWithTag ("Discovery"));
	}

	void currMusic() {
		GameObject[] musics = GameObject.FindGameObjectsWithTag ("Music");
		for (int i = 0; i < musics.Length; i++) { 
			
			musics [i].GetComponent<AudioSource> ().volume = 0.8f;

			if (musics [i].GetComponent<DontDestroy> ().current == false)
			if (DontDestroy.firstTime == false)
				Destroy (musics [i].gameObject);
		}
	}

}
