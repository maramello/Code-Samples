using UnityEngine;
using System.Collections;

public class NetworkCamera : MonoBehaviour {

	public float smoothing = 5f;        // The speed with which the camera will be following.
	public bool survivalMode = false;

	Vector3 offset;                     // The initial offset from the target.

	void Start ()
	{
//		target = GameObject.FindGameObjectWithTag ("Player").transform;
		// Calculate the initial offset.
		if (survivalMode)
			offset = transform.position - getTargetSurvival ();
		else
			offset = transform.position - getTarget();
	}

	Vector3 getTarget() {

		GameObject[] playersInGame = GameObject.FindGameObjectsWithTag ("Player");

		for (int i = 0; i < playersInGame.Length; i++) {
			
			if (playersInGame [i].GetComponent<NetworkPlayer>().isLocalPlayer) 
				return playersInGame [i].transform.position;
		}

		return transform.position;
	}

	Vector3 getTargetSurvival() {

		GameObject[] playersInGame = GameObject.FindGameObjectsWithTag ("SurvivalPlayer");

		for (int i = 0; i < playersInGame.Length; i++) {

			if (playersInGame [i].GetComponent<SurvivalPlayerOnline>().isLocalPlayer) 
				return playersInGame [i].transform.position;
		}

		return transform.position;
	}

	void FixedUpdate ()
	{
		// Create a postion the camera is aiming for based on the offset from the target.
		Vector3 targetCamPos;
		if (survivalMode)
			targetCamPos = getTargetSurvival () + offset;
		else 
			targetCamPos = getTarget() + offset;

		Vector3 actualPos = new Vector3 (targetCamPos.x, targetCamPos.y, -10);
		// Smoothly interpolate between the camera's current position and it's target position.
		transform.position = Vector3.Lerp (transform.position, actualPos, smoothing * Time.deltaTime);
	}
}