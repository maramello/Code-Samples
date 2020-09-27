using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using Prototype.NetworkLobby;

public class CustomDiscovery : NetworkDiscovery {

	public LobbyManager manager;

	public bool exec = false;

	public override void OnReceivedBroadcast(string fromAddress, string data) 
	{
		if (!exec) {
			manager = GameObject.FindGameObjectWithTag ("Manager").GetComponent<LobbyManager> ();

			manager.lobbyPanel.gameObject.SetActive (true);

			manager.networkAddress = fromAddress;
			manager.StartClient ();

			print ("BROADCAST RECEIVED AT " + fromAddress);	

			exec = true;
		}
	}
		
}
