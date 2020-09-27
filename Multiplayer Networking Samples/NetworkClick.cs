using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class NetworkClick : MonoBehaviour {

	public NetworkManager manager;
	public bool isWithNetworkManager = true;
	public CustomDiscovery discovery;
	public Button joinGame;
	public 	Button mainMenuButton;

	// Use this for initialization
	void Start () {
		if (mainMenuButton != null)
		mainMenuButton.gameObject.SetActive (false);
	//	DontDestroyOnLoad (discovery);
	}
	
	// Update is called once per frame
	void Update () {
		if (!isWithNetworkManager)
			return;
		manager = GameObject.FindGameObjectWithTag("Manager").GetComponent<NetworkManager> ();
		discovery = GameObject.FindGameObjectWithTag ("Discovery").GetComponent<CustomDiscovery> ();

	}

	public void onHostGameClick() {
		manager.StartHost ();
	}

	public void onJoinGameClick() {
		discovery.Initialize ();
		discovery.StartAsClient ();
		joinGame.enabled = false;
		joinGame.GetComponentInChildren<Text> ().text = "Connecting...";
		Invoke ("Refresh", 5f);
	}

	public void Refresh() {
		discovery.StopBroadcast ();
		joinGame.enabled = true;
		joinGame.GetComponentInChildren<Text>().text = "Failed. Click To Retry";
	}

	public void onBackClick() {
		
		if (GameObject.FindGameObjectWithTag("Manager") != null)
			Destroy(GameObject.FindGameObjectWithTag("Manager"));

		if (GameObject.FindGameObjectWithTag("Discovery") != null)
			Destroy(GameObject.FindGameObjectWithTag("Discovery"));
		
		SceneManager.LoadScene ("MainMenu", LoadSceneMode.Single);	
		//SceneManager.LoadScene ("ModeSelect", LoadSceneMode.Single);
	}

	public void onMainMenuClick() {
		manager = GameObject.FindGameObjectWithTag ("Manager").GetComponent<NetworkManager>();
		manager.StopHost ();

		SceneManager.LoadScene ("ModeSelect", LoadSceneMode.Single);
	}

	public void oRaceClick() {
		SceneManager.LoadScene ("Lobby", LoadSceneMode.Single);
	}

}
