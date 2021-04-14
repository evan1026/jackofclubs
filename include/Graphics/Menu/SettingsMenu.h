#pragma once

#include "Graphics/Components/HBox.h"
#include "Graphics/Components/Slider.h"
#include "Graphics/Components/Text.h"
#include "Graphics/Components/VBox.h"
#include "Graphics/Menu/Menu.h"

class SettingsMenu : public Menu {

    float lightPos[3];
    std::shared_ptr<Slider<float>> _lightPosXSlider;
    std::shared_ptr<Slider<float>> _lightPosYSlider;
    std::shared_ptr<Slider<float>> _lightPosZSlider;

    std::shared_ptr<Text> _lightPosXText;
    std::shared_ptr<Text> _lightPosYText;
    std::shared_ptr<Text> _lightPosZText;

    std::shared_ptr<HBox> _lightPosXBox;
    std::shared_ptr<HBox> _lightPosYBox;
    std::shared_ptr<HBox> _lightPosZBox;

    std::shared_ptr<VBox> _vbox;

    public:
        SettingsMenu();
};
