#pragma once

#include "Graphics/Components/Slider.h"
#include "Graphics/Menu/Menu.h"

class SettingsMenu : public Menu {

    //Slider<float> _lightPosXSlider;
    //Slider<float> _lightPosYSlider;
    //Slider<float> _lightPosZSlider;

    protected:
        void renderComponent(sf::RenderWindow& w) override;

    public:
        SettingsMenu();

        bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) override;
        bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) override;
        bool handleKeyPressed(const sf::Event::KeyEvent& e) override;
        bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) override;
        bool handleResize(const sf::Event::SizeEvent& e) override;
};
