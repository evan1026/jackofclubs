#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Game.h"
#include "Graphics/Menu/ColorSelectorMenu.h"
#include "Logger/GlobalLogger.hpp"
#include "Rendering/RenderEngine.h"
#include "Utils/Math.h"
#include "Utils/Utils.h"
#include "World/BlockFace.h"
#include "WorldScreen.h"

using Logger::globalLogger;

#define MOUSE_SENSITIVITY 0.1
#define MOVEMENT_SPEED 0.09
#define JUMP_SPEED 0.3
#define GRAVITY_SPEED 0.02
#define SPRINT_SCALE 1.5

/*! \callergraph
 *
 * Construct a new WorldScreen and give it some info about the
 * environment it's running in (window and game). Sets up player's
 * initial position and constructs GUI elements.
 *
 * \p window - The window we're running in (used to help interface with the mouse)    <br>
 * \p game   - The Game being run (used to end the game)                              <br>
 */
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

/*! \callergraph
 *
 * Dispatches events to their appropriate handlers
 *
 * \p event - Event to be handled
 */
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

/*! \callergraph
 *
 * Handles placing/removing blocks if there's no menu or forwarding the
 * mouse click to the menu if there is one.
 *
 * \p event - The mouse click event to handle
 */
void WorldScreen::handleMouseButtonPressed(const sf::Event::MouseButtonEvent& event) {
    if (_activeMenu != nullptr && _activeMenu->handleMouseButtonPressed(event)) {
        return;
    }

    // If the mouse isn't captured at this point, it means there is an
    // active menu which didn't handle the click or we lost focus and
    // just got it back. Either way, calling removeMenu() will do
    // what we want. If it is captured, start adding/removing blocks
    switch (event.button) {
        case sf::Mouse::Button::Left:
            if (!_mouseCaptured) {
                removeMenu();
            } else {
                removeBlock();
            }
            break;
        case sf::Mouse::Button::Right:
            if (!_mouseCaptured) {
                removeMenu();
            } else {
                placeBlock();
            }
            break;
        default:
            break;
    }
}

/*! \callergraph
 *
 * Simply forwards the release event on to the menu. We don't actually
 * care about it.
 * (Done in a format similar to the other functions so that if we do want
 * to handle it in the future, there's less chance of messing it up)
 *
 * \p event - The mouse button release event to handle
 */
void WorldScreen::handleMouseButtonReleased(const sf::Event::MouseButtonEvent& event) {
    if (_activeMenu != nullptr && _activeMenu->handleMouseButtonReleased(event)) {
        return;
    }
}

/*! \callergraph
 *
 * Forwards the key press on to the children, and, if they don't handle it,
 * handles Escape, Q, C, and E.
 *
 * \p event - The key event to handle
 */
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
        case sf::Keyboard::E:
            copySelectionColor();
            break;
        default:
            break;
    }
}

/*! \callergraph
 *
 * Forwards mouse movement on to the menu, if there is one.
 * I tried to have this also handle looking around, but the problem
 * is that re-centering the mouse generates an event in itself, which
 * causes the camera to swing wildly around. The current implementation
 * works much better.
 *
 * \p event - The mouse movement to forward
 */
void WorldScreen::handleMouseMoved(const sf::Event::MouseMoveEvent& event) {
    if (_activeMenu != nullptr && _activeMenu->handleMouseMoved(event)) {
        return;
    }
}

/*! \callergraph
 *
 * Gets the position of the block space next to the block we're looking at
 * (where we should place a block) and then tells the world to make that
 * block solid. Removes the block if it intersected with us. This function
 * may have to be changed slightly when moving to the client-server model.
 */
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

/*! \callergraph
 *
 * Gets the position of the block we're looking at and sets it to air
 */
void WorldScreen::removeBlock() {
    if (!_selectedBlock)
        return;

    sf::Vector3f position = _selectedBlock().getPosition();
    sf::Vector3i blockPosition(position.x, position.y, position.z);

    _world.setBlockType(blockPosition, Block::Type::AIR);
}

/*! \callergraph
 *
 * Shows or hides the color selection menu
 */
void WorldScreen::toggleColorSelectorMenu() {
    if (_activeMenu == nullptr) {
        _mouseCaptured = false;
        _activeMenu = std::unique_ptr<Menu>(new ColorSelectorMenu(_selectedColor));
        _activeMenu->prevMenu = nullptr;
    } else if (_activeMenu->getType() == Menu::Type::ColorSelector) {
        removeMenu();
    }
}

/*! \callergraph
 *
 * Removes any menu that may exists and captures the mouse
 *
 * TODO should probably have a function to go to the previous menu when menu chaining actually starts to happen
 */
void WorldScreen::removeMenu() {
    _activeMenu = nullptr;
    _mouseCaptured = true;
    sf::Mouse::setPosition(_screenMiddle, _window);
}

/*! \callergraph
 *
 * Moves the player and handles the rotation
 */
void WorldScreen::handlePlayerMovement() {
    if (!_mouseCaptured)
        return;

    sf::Vector3f rotation(_player.getRotation());

    if (_mouseCaptured) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(_window);

        // Calculate how far the mouse has moved
        sf::Vector2f diff((mousePos.x - _screenMiddle.x) * MOUSE_SENSITIVITY, (mousePos.y - _screenMiddle.y) * MOUSE_SENSITIVITY);

        // Use that distance to create a rotation
        // Also make sure it stays within bounds
        rotation += sf::Vector3f(diff.y, diff.x, 0.f);
        if (rotation.x > 90) rotation.x = 90;
        else if (rotation.x < -90) rotation.x = -90;

        _player.setRotation(rotation);

        // Recenter the mouse
        sf::Mouse::setPosition(_screenMiddle, _window);
    }

    float speed = MOVEMENT_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
        speed *= SPRINT_SCALE;
    }

    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool forward = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool backward = sf::Keyboard::isKeyPressed(sf::Keyboard::S);

    sf::Vector3f vel;
    if (forward){
        vel.x += speed * Math::sinDeg(rotation.y);
        vel.z += -speed * Math::cosDeg(rotation.y);
    } else if (backward){
        vel.x -= speed * Math::sinDeg(rotation.y);
        vel.z -= -speed * Math::cosDeg(rotation.y);
    }
    if (left){
        vel.x += speed * Math::sinDeg(rotation.y - 90);
        vel.z += -speed * Math::cosDeg(rotation.y - 90);
    } else if (right){
        vel.x += speed * Math::sinDeg(rotation.y + 90);
        vel.z += -speed * Math::cosDeg(rotation.y + 90);
    }

    vel.y = _player.getVelocity().y;
    if (up && !_player.getJumping() && Math::similar(vel.y, 0.0f, 5)) {
        vel.y = JUMP_SPEED;
        _player.setJumping(true);
    }

    vel.y -= GRAVITY_SPEED;

    _player.setVelocity(vel);
}

/*! \callergraph
 *
 * Makes the selected color equal the color of the selected block
 * (called when the user presses E)
 */
void WorldScreen::copySelectionColor() {
    if (_selectedBlock) {
        sf::Vector3f posf = _selectedBlock().getPosition();
        sf::Vector3i posi(posf.x, posf.y, posf.z);
        _selectedColor = _world.getBlockColor(posi);
    }
}

/*! \callergraph
 *
 * Called every frame. Updates FPSCounter, handles player movement, and
 * recalculates selected block.
 */
void WorldScreen::tick() {
    _window.setMouseCursorVisible(!_mouseCaptured);
    _fpsCounter.update();

    if (_activeMenu == nullptr) handlePlayerMovement();

    _player.tick(_world);

    _selectedBlock = _player.getSelection(_world, 5);
}

/*! \callergraph
 *
 * Handles rendering all of the components
 *
 * \p re - Rendering engine used for OpenGL calls    <br>
 * \p w  - Window used for SFML calls                <br>
 */
void WorldScreen::render(RenderEngine& re, sf::RenderWindow& w) {

    // First we calculate the positions for the color and center rectangles
    {
        sf::Vector2u size = w.getSize();
        sf::Vector2f rectSize = _colorRect.getSize();
        float lineThickness = _colorRect.getOutlineThickness();

        _colorRect.setPosition(size.x - rectSize.x - lineThickness, size.y - rectSize.y - lineThickness);
        _colorRect.setFillColor(_selectedColor);

        rectSize = _centerRect.getSize();
        _centerRect.setPosition(size.x / 2 - rectSize.x / 2, size.y / 2 - rectSize.y / 2);
        _centerRect.setFillColor(sf::Color::Black);
    }

    // Next we call render on the renderables
    _player.render(re, w);
    _world.render(re, w);

    // If we've selected a block, draw its outline
    if (_selectedBlock) {
        sf::Vector3f blockPosition = _selectedBlock().getPosition();
        sf::Vector3i blockPositioni = sf::Vector3i(blockPosition.x, blockPosition.y, blockPosition.z);
        AABB blockBox = _world.getBlock(blockPositioni).getBoundingBox();

        // Default outline color is black
        sf::Color outlineColor = sf::Color::Black;

        // But for darker blocks, make it white to improve visibility
        sf::Color blockColor = _world.getBlockColor(blockPositioni);
        if (blockColor.r < 128 && blockColor.g < 128 && blockColor.b < 128) {
            outlineColor = sf::Color::White;
        }

        // And then render the outline
        re.renderBlockSelection(blockBox, outlineColor);
    }

    // Render the FPSCounter (it will check if it should be rendered at all)
    _fpsCounter.render(re, w);

    // Draw our other GUI components
    // TODO probably shouldn't do this if there's a menu up
    w.pushGLStates();
    w.draw(_colorRect);
    w.draw(_centerRect);
    w.popGLStates();

    // And draw the menu if there is one
    if (_activeMenu != nullptr) {
        _activeMenu->render(re, w);
    }
}
