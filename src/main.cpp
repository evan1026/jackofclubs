#include <cmath>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Player.h"
#include "Rendering/RenderEngine.h"
#include "World/World.h"

sf::Vector2i screenMiddle;
bool captured = true;
Player p(Player::Type::SELF, sf::Vector3f(0.f, 140.f, 0.f), sf::Vector3f(0.f, 0.f, 0.f));

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

    //throwException(nullptr, nullptr);

    RenderEngine::init(1920, 1080);
    RenderEngine& e = RenderEngine::getInst();
    std::shared_ptr<sf::Window> window = e.window;
    World::init();

    screenMiddle = sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2);
    sf::Mouse::setPosition(screenMiddle, *window);

    bool running = true;
    while (running){

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
                captured = false;
            }
            else if (event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Button::Left && !captured) {
                    captured = true;
                    sf::Mouse::setPosition(screenMiddle, *window);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F3) {
                    p.setRendered(!p.getRendered());
                } else if (event.key.code == sf::Keyboard::Escape) {
                    captured = false;
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
            running = false;
        }
        if (captured){
            window->setMouseCursorVisible(false);
            doTranslations(*window);
        } else {
            window->setMouseCursorVisible(true);
        }
        e.render(p.getRotation(), p.getPosition());
        window->display();
    }

    return 0;
}

void doTranslations(sf::Window & window){
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2i diffi = mousePos - screenMiddle;

#define SENSITIVITY 0.1

    sf::Vector2f diff = sf::Vector2f(diffi.x * SENSITIVITY, diffi.y * SENSITIVITY);

    sf::Vector3f rotation = p.getRotation();
    rotation += sf::Vector3f(diff.y, diff.x, 0.f);
    if (rotation.x > 90) rotation.x = 90;
    else if (rotation.x < -90) rotation.x = -90;
    p.setRotation(rotation);

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
        vel.x += SPEED * sinDeg(rotation.y);
        vel.z += -SPEED * cosDeg(rotation.y);
    }
    else if (backward){
        vel.x -= SPEED * sinDeg(rotation.y);
        vel.z -= -SPEED * cosDeg(rotation.y);
    }
    if (left){
        vel.x += SPEED * sinDeg(rotation.y - 90);
        vel.z += -SPEED * cosDeg(rotation.y - 90);
    }
    else if (right){
        vel.x += SPEED * sinDeg(rotation.y + 90);
        vel.z += -SPEED * cosDeg(rotation.y + 90);
    }

    p.move(vel);
}

float sinDeg(float a){
    return sin(a * PI / 180.f);
}

float cosDeg(float a){
    return cos(a * PI / 180.f);
}
