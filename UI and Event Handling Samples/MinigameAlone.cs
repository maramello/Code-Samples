using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class MinigameAlone : MonoBehaviour {

	public Button playButton;

	[Space]
	[Header("Map Settings")]
	public GameObject mapPanel;
	public Dropdown mapDropdown;
	public Text mapName;
	public Image mapImage;

	public string[] timeTrialMaps = {"Ruin"};
	public string[] survivalMaps = {"Abandoned"};

	[Space]
	[Header("Game Mode Settings")]
	public Dropdown gameModeDropdown;
	public GameObject questionPanel;
	public Text questionTitle;
	public Text questionText;
	public string[] gameModes = {"Time Trial", "Survival"};
	public string[] gameModeDescriptions = {"How fast of an egg are you ? See how fast you can make it to the end of the course, and beat your personal record.", "Rotten eggs everywhere. They touch you, you rot ! Survive for as long as you can !"};

	private string[][] mapNames;

	// Use this for initialization
	void Start () {
		
		mapNames = new string[gameModes.Length][];
		mapNames [0] = timeTrialMaps;
		mapNames [1] = survivalMaps;
	//	mapNames = new string[gameModes.Length,  Mathf.Max(timeTrialMaps.Length, survivalMaps.Length)];

		playButton.interactable = false;
	}

	// Update is called once per frame
	void Update () {
		
	}

	public void onBackClick() {
		SceneManager.LoadScene ("Minigame", LoadSceneMode.Single);
	}

	public void onTrialBackClick() {
		SceneManager.LoadScene ("ModeSelect", LoadSceneMode.Single);
	}

	public void onSelectMapClick() {
		mapDropdown.ClearOptions ();

		string[] arr = mapNames[gameModeDropdown.value];

		List<string> options = new List<string> (arr);
		mapDropdown.AddOptions (options);
		mapPanel.gameObject.SetActive (true);
		mapImage.sprite = Resources.Load("Maps/" + mapNames[gameModeDropdown.value][mapDropdown.value], typeof(Sprite)) as Sprite;
	}

	public void onMapSelect() {
		mapPanel.gameObject.SetActive (false);
		mapName.text = mapNames [gameModeDropdown.value][mapDropdown.value];
		playButton.interactable = true;
	}

	public void onMapChanged() {
		mapName.text = mapNames [gameModeDropdown.value][mapDropdown.value];
		mapImage.sprite = Resources.Load("Maps/" + mapNames[gameModeDropdown.value][mapDropdown.value], typeof(Sprite)) as Sprite;
	}

	public void onGameModeChanged() {
		playButton.interactable = false;
		mapName.text = "Select Map";
	}

	public void onPlayClick() {
		SceneManager.LoadScene (mapName.text, LoadSceneMode.Single);
	}

	public void onQuestionClick() {
		questionTitle.text = gameModes [gameModeDropdown.value];
		questionText.text = gameModeDescriptions [gameModeDropdown.value];
		questionPanel.gameObject.SetActive (true);
	}

	public void onQuestionClose() {
		questionPanel.gameObject.SetActive (false);
	}

}
