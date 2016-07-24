#include <cmath>
#include <iostream>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <sstream>

#include "Block.h"
#include "RenderEngine.h"

sf::Vector3f cameraPos = sf::Vector3f(0.f, 0.f, -200.f);
sf::Vector3f cameraRot = sf::Vector3f(0.f, 0.f, 0.f);
sf::Vector2i screenMiddle;
bool focused = true;

std::string intToString(int a);
std::string floatToString(float a);
void draw();
void doTranslations(sf::Window & window);
float sinDeg(float a);
float cosDeg(float a);
int doMain();

#define PI    3.1415926535897932384626433832795
#define SPEED 10

using namespace std;

void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar ) {
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * PI ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

int main() {
    try {
        return doMain();
    } catch (exception& e) {
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

void poop(char * no, RenderEngine* yes) {
    RenderEngine& e = RenderEngine::getInst();
    e.setPerspective(0,0,0,0);
}

int doMain(){

    //poop(nullptr, nullptr);

    RenderEngine::init();
    std::shared_ptr<sf::Window> window = RenderEngine::getInst().window;


    screenMiddle = sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2);
    sf::Mouse::setPosition(screenMiddle, *window);
    window->setMouseCursorVisible(false);

    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspectiveGL(90.f, 1.f, 1.f, 5000.f);

    Block b1(sf::Vector3f(0.f, 0.f, 0.f), sf::Color(0, 255, 0));
    Block b2(sf::Vector3f(50.f, 50.f, 50.f), sf::Color(128, 0, 128));

    bool running = true;
    while (running){
        sf::Event event;
        while (window->pollEvent(event)){
            if (event.type == sf::Event::Closed){
                running = false;
            }
            else if (event.type == sf::Event::Resized){
                glViewport(0, 0, event.size.width, event.size.height);
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

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            draw();
        }

        window->display();
    }

    return 0;
}

string intToString(int a){
    stringstream ss;
    ss << a;
    return ss.str();
}
string floatToString(float a){
    stringstream ss;
    ss << a;
    return ss.str();
}

void draw(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(cameraRot.x, 1.f, 0.f, 0.f);
    glRotatef(cameraRot.y, 0.f, 1.f, 0.f);
    glRotatef(cameraRot.z, 0.f, 0.f, 1.f);
    glTranslatef(cameraPos.x, cameraPos.y, cameraPos.z);

    RenderEngine::getInst().render();
}

void doTranslations(sf::Window & window){
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2i diff = mousePos - screenMiddle;

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

    cameraPos.y += (up) ? -SPEED : (down) ? SPEED : 0;
    if (forward){
        cameraPos.x += -SPEED * sinDeg(cameraRot.y);
        cameraPos.z += SPEED * cosDeg(cameraRot.y);
    }
    else if (backward){
        cameraPos.x -= -SPEED * sinDeg(cameraRot.y);
        cameraPos.z -= SPEED * cosDeg(cameraRot.y);
    }
    if (left){
        cameraPos.x += -SPEED * sinDeg(cameraRot.y - 90);
        cameraPos.z += SPEED * cosDeg(cameraRot.y - 90);
    }
    else if (right){
        cameraPos.x += -SPEED * sinDeg(cameraRot.y + 90);
        cameraPos.z += SPEED * cosDeg(cameraRot.y + 90);
    }
}

float sinDeg(float a){
    return sin(a * PI / 180.f);
}
float cosDeg(float a){
    return cos(a * PI / 180.f);
}
