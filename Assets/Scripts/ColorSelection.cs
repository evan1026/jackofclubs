using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityStandardAssets.Characters.FirstPerson;

public class ColorSelection : Singleton<ColorSelection> {

	private Color selectedColor;

	public ColorPreviewImageUpdater preview;

	public GameObject ColorSelectionPanel;
	public Image SelectedColorPreview;
	public Slider redSlider;
	public Slider greenSlider;
	public Slider blueSlider;
	public FirstPersonController fpc;

	// Use this for initialization
	void Start () {
		selectedColor = new Color (255, 255, 255, 255);
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetButtonDown("Select New Color")) {
			if (ColorSelectionPanel.activeSelf) {
				setColor (SelectedColorPreview.color);
				ColorSelectionPanel.SetActive (false);
				fpc.setCursorLock (true);
			} else {
				ColorSelectionPanel.SetActive (true);
				redSlider.value = selectedColor.r * 255;
				greenSlider.value = selectedColor.g * 255;
				blueSlider.value = selectedColor.b * 255;
				fpc.setCursorLock (false);
			}
		}

		if (Input.GetButtonDown("Copy Color")) {
			RaycastHit selectedBlock = RayCasting.Instance.getSelectedBlock ();
			if (selectedBlock.collider != null) {
				Block b = TerrainHelper.GetBlock (selectedBlock);
				setColor (b.color);
			}
		}
	}

	private void setColor(Color newColor) {
		selectedColor = newColor;
		preview.setColor (newColor);

	}

	public Color getColor() {
		return selectedColor;
	}
}
