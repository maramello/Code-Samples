using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using AppAdvisory.social;

public class Tips : MonoBehaviour {

	public GameObject tipsPanel;

	public Text titleText;
	public Text descriptionText;
	public Image image;
	public Text leftButtonText;

	public string[] tipDescriptions;
	public Sprite[] images;
	public string[] leftButtonTexts;
	public string[] scenesToGoTo;

	private int index;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void display() {

		index = Random.Range (0, tipDescriptions.Length);

		descriptionText.text = tipDescriptions [index];
		image.sprite = images [index];
		leftButtonText.text = leftButtonTexts [index];

		tipsPanel.gameObject.SetActive (true);
	}

	public void onShopClick() {

		if (scenesToGoTo [index] == "Leaderboard")
			LeaderboardManager.ShowLeaderboardUI ();
		else
			SceneManager.LoadScene (scenesToGoTo[index], LoadSceneMode.Single);

		tipsPanel.gameObject.SetActive (false);

	}

	public void onCloseClick() {
		tipsPanel.gameObject.SetActive (false);
	}
}
