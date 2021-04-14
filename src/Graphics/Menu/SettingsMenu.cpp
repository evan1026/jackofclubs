#include "Graphics/Menu/SettingsMenu.h"

#define WIDTH  500
#define HEIGHT 500

SettingsMenu::SettingsMenu() : Menu(sf::Vector2f(WIDTH, HEIGHT), Menu::Type::Settings),
    lightPos{0., 0., 0.}, 
    _lightPosXSlider(std::make_shared<Slider<float>>(-1., 1., lightPos[0], sf::Vector2i(0,0), sf::Vector2i(200,50), Slider<float>::BAR_DIRECTION::HORIZONTAL, Slider<float>::TEXT_LOCATION::RIGHT)),
    _lightPosYSlider(std::make_shared<Slider<float>>(-1., 1., lightPos[1], sf::Vector2i(0,0), sf::Vector2i(200,50), Slider<float>::BAR_DIRECTION::HORIZONTAL, Slider<float>::TEXT_LOCATION::RIGHT)),
    _lightPosZSlider(std::make_shared<Slider<float>>(-1., 1., lightPos[2], sf::Vector2i(0,0), sf::Vector2i(200,50), Slider<float>::BAR_DIRECTION::HORIZONTAL, Slider<float>::TEXT_LOCATION::RIGHT)),
    _lightPosXText(std::make_shared<Text>(sf::Vector2i(0,0), "Light Position X Value:")),
    _lightPosYText(std::make_shared<Text>(sf::Vector2i(0,0), "Light Position Y Value:")),
    _lightPosZText(std::make_shared<Text>(sf::Vector2i(0,0), "Light Position Z Value:")),
    _lightPosXBox(std::make_shared<HBox>(std::initializer_list<std::shared_ptr<Component>>{_lightPosXText, _lightPosXSlider})),
    _lightPosYBox(std::make_shared<HBox>(std::initializer_list<std::shared_ptr<Component>>{_lightPosYText, _lightPosYSlider})),
    _lightPosZBox(std::make_shared<HBox>(std::initializer_list<std::shared_ptr<Component>>{_lightPosZText, _lightPosZSlider})),
    _vbox(std::make_shared<VBox>(std::initializer_list<std::shared_ptr<Component>>{_lightPosXBox, _lightPosYBox, _lightPosZBox}))
{
    add(_vbox);
}
