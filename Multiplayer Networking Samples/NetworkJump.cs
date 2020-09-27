using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class NetworkJump : MonoBehaviour {
	
	public GameObject player;

	public void Jump() {

		player = correctPlayer ();

		if (player == null)
			return;
		
		if (player.GetComponent<NetworkPlayer> ().dead)
			return;
		if (player.GetComponent<NetworkPlayer> ().jumpCount >= 2)
			return;
		
		player.GetComponent<Rigidbody2D>().AddForce (Vector2.up * 500);
		player.GetComponent<NetworkPlayer> ().jumpCount++;
	}

	private GameObject correctPlayer() {
		GameObject manager = GameObject.FindGameObjectWithTag ("Manager");
		if (manager != null)
			print ("NETWORK MANAGER FOUND");
		else
			print ("NETWORK MANAGER NOT FOUND");
		
		GameObject[] playersInGame = GameObject.FindGameObjectsWithTag ("Player");

		for (int i = 0; i < playersInGame.Length; i++) {
			if (playersInGame [i].GetComponent<NetworkPlayer> ().isLocalPlayer == true)
				return playersInGame [i];
		}
		return null;
	}

	public void survivalJump() {
		player = survivalPlayer ();

		if (player == null)
			return;

		if (player.GetComponent<SurvivalPlayerOnline> ().dead)
			return;

		if (player.GetComponent<SurvivalPlayerOnline> ().jumpCount >= 2)
			return;

		player.GetComponent<Rigidbody2D>().AddForce (Vector2.up * 500);
		player.GetComponent<SurvivalPlayerOnline> ().jumpCount++;
	}

	private GameObject survivalPlayer() {
		GameObject[] playersInGame = GameObject.FindGameObjectsWithTag ("SurvivalPlayer");

		for (int i = 0; i < playersInGame.Length; i++) {
			if (playersInGame [i].GetComponent<SurvivalPlayerOnline> ().isLocalPlayer == true)
				return playersInGame [i];
		}
		return null;
	}

	public void stopHosting() {
		GameObject manager = GameObject.FindGameObjectWithTag ("Manager");
		GameObject discovery = GameObject.FindGameObjectWithTag ("Discovery");

		if (manager != null) {
			print ("stopping server..");
			manager.GetComponent<NetworkLobbyManager> ().StopHost ();
			manager.GetComponent<NetworkLobbyManager> ().networkAddress = "localhost";
		} else {
			print ("NETWORK MANAGER NOT FOUND");
		}

		if (discovery != null)
			Destroy (discovery);
	}
}
