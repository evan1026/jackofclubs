#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Game.h"
#include "Graphics/Menu/ColorSelectorMenu.h"
#include "Rendering/RenderEngine.h"
#include "Utils/Math.h"
#include "Utils/Utils.h"
#include "World/BlockFace.h"
#include "WorldScreen.h"

#define MOUSE_SENSITIVITY 0.1
#define MOVEMENT_SPEED 0.1

WorldScreen::WorldScreen(sf::RenderWindow& window, Game& game) :
    _world(),
    _player(Player::Type::SELF, sf::Vector3f(0.f, 140.f, 0.f), sf::Vector3f(0.f, 0.f, 0.f)),
    _fpsCounter(sf::Vector2i(5, 0)),
    _activeMenu(nullptr),
    _colorRect(sf::Vector2f(100, 100)),
    _centerRect(sf::Vector2f(4, 4)),
    _mouseCaptured(true),
    _screenMiddle(window.getSize().x / 2, window.getSize().y / 2),
    _selectedColor(sf::Color::White),
    _selectedBlock(),
    _window(window),
    _game(game)
{
    _colorRect.setOutlineColor(sf::Color::Black);
    _colorRect.setOutlineThickness(2);
}

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
        case sf::Event::MouseButtonReleased:
            handleMouseButtonReleased(event.mouseButton);
            break;
        case sf::Event::KeyPressed:
            handleKeyPressed(event.key);
            break;
        case sf::Event::MouseMoved:
            handleMouseMoved(event.mouseMove);
            break;
        default:
            break;
    }
}

void WorldScreen::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& event) {
    if (_activeMenu != nullptr && _activeMenu->handleMouseButtonPressed(event)) {
        return;
    }

    switch (event.button) {
        case sf::Mouse::Button::Left:
            if (!_mouseCaptured) {
                removeMenu();
            } else {
                removeBlock();
            }
            break;
        case sf::Mouse::Button::Right:
            placeBlock();
            break;
        default:
            break;
    }
}

void WorldScreen::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& event) {
    if (_activeMenu != nullptr && _activeMenu->handleMouseButtonReleased(event)) {
        return;
    }
}

void WorldScreen::handleKeyPressed(const sf::Event::KeyEvent& event) {
    if (_activeMenu != nullptr && _activeMenu->handleKeyPressed(event)) {
        return;
    }

    switch (event.code) {
        case sf::Keyboard::Escape:
            _mouseCaptured = false;
            break;
        case sf::Keyboard::Q:
            _game.end();
            break;
        case sf::Keyboard::C:
            toggleColorSelectorMenu();
            break;
        default:
            break;
    }
}

void WorldScreen::handleMouseMoved(const sf::Event::MouseMoveEvent& event) {
    if (_activeMenu != nullptr && _activeMenu->handleMouseMoved(event)) {
        return;
    }
}

void WorldScreen::placeBlock() {
    if (!_selectedBlock)
        return;

    sf::Vector3f position = _selectedBlock().getPosition() + _selectedBlock().getNormal();
    sf::Vector3i blockPosition(position.x, position.y, position.z);

    _world.setBlockType(blockPosition, Block::Type::SOLID);
    _world.setBlockColor(blockPosition, _selectedColor);

    if (_world.checkCollision(_player))
        _world.setBlockType(blockPosition, Block::Type::AIR);
}

void WorldScreen::removeBlock() {
    if (!_selectedBlock)
        return;

    sf::Vector3f position = _selectedBlock().getPosition();
    sf::Vector3i blockPosition(position.x, position.y, position.z);

    _world.setBlockType(blockPosition, Block::Type::AIR);
}

void WorldScreen::toggleColorSelectorMenu() {
    if (_activeMenu == nullptr) {
        _mouseCaptured = false;
        std::unique_ptr<Menu> prev = std::move(_activeMenu);
        _activeMenu = std::unique_ptr<Menu>(new ColorSelectorMenu(_selectedColor));
        _activeMenu->prevMenu = std::move(prev);
    } else if (_activeMenu->getType() == Menu::Type::ColorSelector) {
        removeMenu();
    }
}

void WorldScreen::removeMenu() {
    if (_activeMenu != nullptr) _activeMenu = std::move(_activeMenu->prevMenu);
    _mouseCaptured = true;
    sf::Mouse::setPosition(_screenMiddle, _window);
}

void WorldScreen::handlePlayerMovement() {
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
}

void WorldScreen::tick() {
    _window.setMouseCursorVisible(!_mouseCaptured);
    _fpsCounter.update();

    if (_activeMenu == nullptr) handlePlayerMovement();

    _selectedBlock = _player.getSelection(_world, 5);
}

void WorldScreen::render(RenderEngine& re, sf::RenderWindow& w) {

    sf::Vector2u size = w.getSize();
    sf::Vector2f rectSize = _colorRect.getSize();
    float lineThickness = _colorRect.getOutlineThickness();

    _colorRect.setPosition(size.x - rectSize.x - lineThickness, size.y - rectSize.y - lineThickness);
    _colorRect.setFillColor(_selectedColor);

    rectSize = _centerRect.getSize();
    _centerRect.setPosition(size.x / 2 - rectSize.x / 2, size.y / 2 - rectSize.y / 2);
    _centerRect.setFillColor(sf::Color::Black);

    _player.render(re, w);
    _world.render(re, w);
    if (_selectedBlock) {
        sf::Vector3f blockPosition = _selectedBlock().getPosition();
        sf::Vector3i blockPositioni = sf::Vector3i(blockPosition.x, blockPosition.y, blockPosition.z);
        AABB blockBox = _world.getBlock(blockPositioni).getBoundingBox();
        sf::Color outlineColor = sf::Color::Black;
        sf::Color blockColor = _world.getBlockColor(blockPositioni);
        if (blockColor.r < 128 && blockColor.g < 128 && blockColor.b < 128) {
            outlineColor = sf::Color::White;
        }
        re.renderBlockSelection(blockBox, outlineColor);
    }
    _fpsCounter.render(re, w);

    w.pushGLStates();
    w.draw(_colorRect);
    w.draw(_centerRect);
    w.popGLStates();

    if (_activeMenu != nullptr) {
        _activeMenu->render(re, w);
    }
}
