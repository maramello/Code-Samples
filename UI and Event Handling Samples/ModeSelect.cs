using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine.Analytics;
using UnityEngine.Networking;

public class ModeSelect : MonoBehaviour {

	public GameObject messagePanel;
	public Text msg;
	public Message message;
	public Button restoreButton;

	// Use this for initialization
	void Start () {
		
		PlayerPrefs.SetString("Multiplayer", "Yes");

		if (Application.platform == RuntimePlatform.Android)
			restoreButton.gameObject.SetActive (false);

		if (GameObject.FindGameObjectWithTag ("Manager") != null) {
			GameObject.FindGameObjectWithTag ("Manager").GetComponent<NetworkManager> ().StopHost ();
			Destroy (GameObject.FindGameObjectWithTag ("Manager"));
		}
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void onClassicClick() {
		analyzeClick ("Classic");
		SceneManager.LoadScene ("LevelInfinite", LoadSceneMode.Single);
	}

	public void onTimeTrialClick() {
		if (PlayerPrefs.GetInt ("HighScore") >= 25) {
			analyzeClick ("Time Trial");
			SceneManager.LoadScene ("TimeTrialSelect", LoadSceneMode.Single);
		}
		else
			display ("This mode is currently locked, get a score of 25 or above in classic to unlock it :)");
	}

	public void onSurvivalClick() {
		if (PlayerPrefs.GetInt ("HighScore") >= 50) {
			analyzeClick ("Survival");
			SceneManager.LoadScene ("SurvivalSelect", LoadSceneMode.Single);
		}
		else
			display ("This mode is currently locked, get a score of 50 or above in classic to unlock it :)");
	}

	public void onMultiplayerClick() {
		analyzeClick ("Multiplayer");

		if (PlayerPrefs.GetString ("Multiplayer") == "Yes")
			SceneManager.LoadScene ("Lobby", LoadSceneMode.Single);
		else
			message.display ("Multiplayer", "You dont have multiplayer right now!");
	}

	private void analyzeClick(string mode) {
		
		Analytics.CustomEvent("Mode Select", new Dictionary<string, object>
			{
				{ "Mode", mode}
			});
	}

	public void display(string message) {
		msg.text = message;
		messagePanel.gameObject.SetActive (true);
	}

	public void onDoneClick() {
		messagePanel.gameObject.SetActive (false);
	}

}
