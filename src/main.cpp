#include <cmath>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Player.h"
#include "Rendering/RenderEngine.h"
#include "World/World.h"

sf::Vector3f cameraPos = sf::Vector3f(0.f, 140.f, 0.f);
sf::Vector3f cameraRot = sf::Vector3f(0.f, 0.f, 0.f);
sf::Vector2i screenMiddle;
bool focused = true;
Player p(Player::Type::SELF, cameraPos, cameraRot);

void doTranslations(sf::Window & window);
float sinDeg(float a);
float cosDeg(float a);
int doMain();

#define PI    3.1415926535897932384626433832795
#define SPEED 0.6

int main() {
    try {
        return doMain();
    } catch (std::exception& e) {
        std::cout << std::endl;
        std::cout << "***************************************" << std::endl;
        std::cout << "* Exception uncaught. Must terminate. *" << std::endl;
        std::cout << "*      Exception details follow       *" << std::endl;
        std::cout << "***************************************" << std::endl;
        std::cout << e.what() << std::endl;
        std::cout << "If you're seeing this in production, please screenshot and send to the developer." << std::endl;
        return 1;
    }
}

//Used to test exception handling
void throwException(char * no, RenderEngine* yes) {
    RenderEngine& e = RenderEngine::getInst();
    e = e; //to suppress warnings
}

int doMain(){

    cameraPos += sf::Vector3f(0.5f, 0.f, 0.5f);

    //throwException(nullptr, nullptr);

    RenderEngine::init(1920, 1080);
    RenderEngine& e = RenderEngine::getInst();
    std::shared_ptr<sf::Window> window = e.window;
    World::init();

    screenMiddle = sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2);
    sf::Mouse::setPosition(screenMiddle, *window);
    window->setMouseCursorVisible(false);

    int x = -32, y = 0, z = -32;

    bool running = true;
    while (running){
        sf::Vector3i pos(x, y, z);

        if (++x == 32) {
            x = -32;
            if (++y == 128) {
                y = 0;
                if (++z == 32) {
                    z = -32;
                }
            }
        }

        World::getInst().setBlockType(pos, Block::Type::AIR);

        sf::Event event;
        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                running = false;
            }
            else if (event.type == sf::Event::Resized){
                e.handleResize(event.size.width, event.size.height);
                screenMiddle = sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2);
            }
            else if (event.type == sf::Event::LostFocus){
                focused = false;
            }
            else if (event.type == sf::Event::GainedFocus){
                focused = true;
                sf::Mouse::setPosition(screenMiddle, *window);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            running = false;
        }
        if (focused){
            doTranslations(*window);

            e.render(cameraRot, cameraPos);
        }

        window->display();
    }

    return 0;
}

void doTranslations(sf::Window & window){
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2i diffi = mousePos - screenMiddle;

#define SENSITIVITY 0.1

    sf::Vector2f diff = sf::Vector2f(diffi.x * SENSITIVITY, diffi.y * SENSITIVITY);

    cameraRot += sf::Vector3f(diff.y, diff.x, 0.f);
    if (cameraRot.x > 90) cameraRot.x = 90;
    else if (cameraRot.x < -90) cameraRot.x = -90;

    sf::Mouse::setPosition(screenMiddle, window);

    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    bool forward = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool backward = sf::Keyboard::isKeyPressed(sf::Keyboard::S);

    sf::Vector3f vel;
    vel.y += (up) ? SPEED : (down) ? -SPEED : 0;
    if (forward){
        vel.x += SPEED * sinDeg(cameraRot.y);
        vel.z += -SPEED * cosDeg(cameraRot.y);
    }
    else if (backward){
        vel.x -= SPEED * sinDeg(cameraRot.y);
        vel.z -= -SPEED * cosDeg(cameraRot.y);
    }
    if (left){
        vel.x += SPEED * sinDeg(cameraRot.y - 90);
        vel.z += -SPEED * cosDeg(cameraRot.y - 90);
    }
    else if (right){
        vel.x += SPEED * sinDeg(cameraRot.y + 90);
        vel.z += -SPEED * cosDeg(cameraRot.y + 90);
    }

    cameraPos += p.move(vel);
}

float sinDeg(float a){
    return sin(a * PI / 180.f);
}

float cosDeg(float a){
    return cos(a * PI / 180.f);
}
