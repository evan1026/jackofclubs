#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Game.h"
#include "Rendering/RenderEngine.h"
#include "Utils/Math.h"
#include "Utils/Utils.h"
#include "WorldScreen.h"

#define MOUSE_SENSITIVITY 0.1
#define MOVEMENT_SPEED 0.6

WorldScreen::WorldScreen(sf::RenderWindow& window, Game& game) :
    _world(),
    _player(Player::Type::SELF, sf::Vector3f(0.f, 140.f, 0.f), sf::Vector3f(0.f, 0.f, 0.f)),
    _fpsCounter(sf::Vector2i(5, 0)),
    _mouseCaptured(true),
    _screenMiddle(window.getSize().x / 2, window.getSize().y / 2),
    _window(window),
    _game(game)
{}

void WorldScreen::handleEvent(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Resized:
            _screenMiddle = sf::Vector2i(event.size.width / 2, event.size.height / 2);
            break;
        case sf::Event::LostFocus:
            _mouseCaptured = false;
            break;
        case sf::Event::MouseButtonPressed:
            handleMouseButtonPressed(event.mouseButton);
            break;
        case sf::Event::KeyPressed:
            handleKeyPressed(event.key);
            break;
        default:
            break;
    }
}

void WorldScreen::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& event) {
    switch (event.button) {
        case sf::Mouse::Button::Left:
            if (!_mouseCaptured) {
                _mouseCaptured = true;
                sf::Mouse::setPosition(_screenMiddle, _window);
            }
            break;
        default:
            break;
    }
}

void WorldScreen::handleKeyPressed(const sf::Event::KeyEvent& event) {
    switch (event.code) {
        case sf::Keyboard::Escape:
            _mouseCaptured = false;
            break;
        case sf::Keyboard::Q:
            _game.end();
            break;
        default:
            break;
    }
}

void WorldScreen::tick() {
    sf::Vector3f rotation(_player.getRotation());

    if (_mouseCaptured) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(_window);

        sf::Vector2f diff((mousePos.x - _screenMiddle.x) * MOUSE_SENSITIVITY, (mousePos.y - _screenMiddle.y) * MOUSE_SENSITIVITY);

        rotation += sf::Vector3f(diff.y, diff.x, 0.f);
        if (rotation.x > 90) rotation.x = 90;
        else if (rotation.x < -90) rotation.x = -90;
        _player.setRotation(rotation);

        sf::Mouse::setPosition(_screenMiddle, _window);
    }

    _window.setMouseCursorVisible(!_mouseCaptured);

    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    bool forward = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool backward = sf::Keyboard::isKeyPressed(sf::Keyboard::S);

    sf::Vector3f vel;
    vel.y += (up) ? MOVEMENT_SPEED : (down) ? -MOVEMENT_SPEED : 0;
    if (forward){
        vel.x += MOVEMENT_SPEED * Math::sinDeg(rotation.y);
        vel.z += -MOVEMENT_SPEED * Math::cosDeg(rotation.y);
    } else if (backward){
        vel.x -= MOVEMENT_SPEED * Math::sinDeg(rotation.y);
        vel.z -= -MOVEMENT_SPEED * Math::cosDeg(rotation.y);
    }
    if (left){
        vel.x += MOVEMENT_SPEED * Math::sinDeg(rotation.y - 90);
        vel.z += -MOVEMENT_SPEED * Math::cosDeg(rotation.y - 90);
    } else if (right){
        vel.x += MOVEMENT_SPEED * Math::sinDeg(rotation.y + 90);
        vel.z += -MOVEMENT_SPEED * Math::cosDeg(rotation.y + 90);
    }

    _player.move(vel, _world);

    _fpsCounter.update();
}

void WorldScreen::render(RenderEngine& re, sf::RenderWindow& w) {
    _player.render(re, w);
    _world.render(re, w);
    _fpsCounter.render(re, w);
}
