using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Freeze : MonoBehaviour {

	public Player eggPlayerTofly;

	public Slider meter;
	public Button flyButton;

	public float decreaseVal = 0.2f;
	public float increaseVal = 0.1f;
	public float updateDelay = 0.1f;
	public float maxValue;

	private bool recharging;

	// initialization
	void Start () {

		if (PlayerPrefs.GetString ("EquippedEgg") == "lagger egg")
			gameObject.SetActive (true);
		else
			gameObject.SetActive (false);

		meter.value = 0;
		meter.maxValue = maxValue;
		flyButton.interactable = false;
		recharging = true;

		Invoke ("rechargeUpdate", updateDelay);
	}

	// Update is called once per frame
	void Update () {

	}

	public void onFlyClick() {
		
		print ("fly started");

		GameObject.Find ("UI Sound").GetComponent<UISound> ().playAbilityActivateSound();

		Time.timeScale = 0.5f;

		recharging = false;
		Invoke ("meterDownUpdate", updateDelay);
	}

	public void meterDownUpdate() {
		meter.value = meter.value - decreaseVal;

	}

	public void onMeterValueUpdated() {

		if (meter.value == meter.maxValue)
			flyButton.interactable = true;
		else
			flyButton.interactable = false;

		//RECHARGING
		if (recharging) {

			if (meter.value == meter.maxValue) {
				recharging = false;
				return;
			}

			Time.timeScale = 1.0f;

			Invoke ("rechargeUpdate", updateDelay);
		} 
		//FLYING
		else {
			if (meter.value <= meter.minValue || meter.value == 0) {
				recharging = true;
				GameObject.Find ("UI Sound").GetComponent<UISound> ().playAbilityOverSound();
				Invoke ("rechargeUpdate", updateDelay);
			} 
			else {
				Invoke ("meterDownUpdate", updateDelay);
			}
		}
	}

	public void rechargeUpdate() {
		meter.value = meter.value + increaseVal;
	}
}
