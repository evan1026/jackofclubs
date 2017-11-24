using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityStandardAssets.Characters.FirstPerson;

public class ColorSelection : Singleton<ColorSelection> {

	private Color selectedColor;

	public Image preview;
	public Animator previewParent;

	public GameObject ColorSelectionPanel;
	public Slider redSlider;
	public Slider greenSlider;
	public Slider blueSlider;
	public FirstPersonController fpc;

	// Use this for initialization
	void Start () {
		setColor(new Color (1, 1, 1, 1));
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetButtonDown("Select New Color")) {
			if (ColorSelectionPanel.activeSelf) {
				setColor (preview.color);
				ColorSelectionPanel.SetActive (false);
				previewParent.SetTrigger ("CloseColorMenu");
				fpc.setCursorLock (true);
			} else {
				previewParent.SetTrigger ("OpenColorMenu");
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
		preview.color = newColor;
		selectedColor = newColor;
	}

	public Color getColor() {
		return selectedColor;
	}
}
