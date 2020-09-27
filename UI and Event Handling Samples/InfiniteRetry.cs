using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using AppAdvisory.social;
using UnityEngine.Advertisements;
using UnityEngine.Analytics;

public class InfiniteRetry : MonoBehaviour {

	public InfinitePlayer player;
	public GameObject retryPanel;
	public GameObject secondLifePanel;
	public Text secondLifeScoreText;
	public Text coinText;
	public Text scoreText;
	public Text displayBestScoreText;
	public Text displayCurrentScoreText;
	public Tips tip;

	private bool processed;

	// Use this for initialization
	void Start () {
		processed = false;
		retryPanel.gameObject.SetActive (false);
	}

	// Update is called once per frame
	void Update () {
		if (player.dead) {

			if (GameObject.FindGameObjectWithTag("Music") != null)
				GameObject.FindGameObjectWithTag ("Music").GetComponent<AudioSource> ().volume = 0.2f;

			retryPanel.gameObject.SetActive (true);
			displayScoreData ();

		}
	}

	public void ShowNormalAd()
	{

		if (!Advertisement.isInitialized) {
			print ("ad not initialized");
			if (Application.platform == RuntimePlatform.IPhonePlayer)
				Advertisement.Initialize ("1482921", true);
			else if (Application.platform == RuntimePlatform.Android)
				Advertisement.Initialize ("1482920", true);
		}

		if (Advertisement.IsReady("video"))
		{
			Advertisement.Show ("video");
		}

	}

	public void onSecondLifeAdClick() {

		Analytics.CustomEvent("Second Life", new Dictionary<string, object>
			{ 
				{ "score", int.Parse (scoreText.text)},
				{ "egg", PlayerPrefs.GetString("EquippedEgg") }
			});


		if (!Advertisement.isInitialized) {
			print ("ad not initialized");
			if (Application.platform == RuntimePlatform.IPhonePlayer)
				Advertisement.Initialize ("1482921", true);
			else if (Application.platform == RuntimePlatform.Android)
				Advertisement.Initialize ("1482920", true);
		}

		if (Advertisement.IsReady("rewardedVideo"))
		{
			var options = new ShowOptions { resultCallback = HandleShowResult };
			Advertisement.Show("rewardedVideo", options);
		}
	}

	public void closeSecondLifeClick() {
		secondLifePanel.gameObject.SetActive (false);
	}

	private void HandleShowResult(ShowResult result)
	{
		switch (result)
		{
		case ShowResult.Finished:
			Debug.Log ("The ad was successfully shown.");
			//
			// YOUR CODE TO REWARD THE GAMER

			Game.previousScore = int.Parse (scoreText.text);
			Game.hasSecondLife = true;

			onRetryClick ();

			break;
		case ShowResult.Skipped:
			Debug.Log("The ad was skipped before reaching the end.");
			break;
		case ShowResult.Failed:
			Debug.LogError("The ad failed to be shown.");
			break;
		}
	}

	public void onRetryClick() {

		CoinManager.numberOfCoins = 0;
		//VERY IMPORTANT
		ChunkIndex.currChunkIndex = 0;
		Generator.numberOfChunks = 0;
		saveData ();
		if (GameObject.FindGameObjectWithTag("Music") != null)
			GameObject.FindGameObjectWithTag ("Music").GetComponent<AudioSource> ().volume = 0.7f;
		SceneManager.LoadScene ("LevelInfinite", LoadSceneMode.Single);
	}

	public void onQuitClick() {
		saveData ();
		SceneManager.LoadScene ("MainMenu", LoadSceneMode.Single);
	}

	private void displayScoreData() {
		//Save score and display it
		int score = int.Parse(scoreText.text);

		displayCurrentScoreText.text = score + "";

		if (score > PlayerPrefs.GetInt ("HighScore")) {
			PlayerPrefs.SetInt ("HighScore", score);
			LeaderboardManager.ReportScore (score);
		}

		displayBestScoreText.text = "Best : " + PlayerPrefs.GetInt ("HighScore");

		if (processed == false) {
			processed = true;

			Analytics.CustomEvent("Classic Game", new Dictionary<string, object>
				{ 
					{ "score", score},
					{ "egg", PlayerPrefs.GetString("EquippedEgg") }
				});

			if (Game.hasSecondLife) {
				print ("just had second life. cancelling.");
				Game.hasSecondLife = false;
				Game.previousScore = 0;
				return;
			}

			else if (Advertisement.IsReady ("rewardedVideo") && int.Parse(scoreText.text) < 500) {
				secondLifeScoreText.text = "" + int.Parse (scoreText.text);
				secondLifePanel.gameObject.SetActive (true);
			}

			else 
				AdProcess ();
		}



	}

	public void AdProcess()
	{

		Game.gameCount = Game.gameCount + 1;

		if (Game.gameCount == 4) {
			if (Random.Range (0, 2) == 1)
				tip.display ();
		}

		if (Game.gameCount >= 5) {
			Game.gameCount = 0;
			ShowNormalAd ();
		}

	}

	private void saveData() {
		//Save coins
		string coinsString = coinText.text.Substring (2);
		int collectedCoins = int.Parse (coinsString);
		PlayerPrefs.SetInt ("Coin", PlayerPrefs.GetInt ("Coin") + collectedCoins);
		coinText.text = ": 0";
		CoinManager.numberOfCoins = 0;


	}
}
