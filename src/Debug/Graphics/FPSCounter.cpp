#include <SFML/Graphics.hpp>

#include "Debug/DebugOptions.h"
#include "Debug/Graphics/FPSCounter.h"
#include "Utils/Font.h"
#include "Utils/Utils.h"

FPSCounter::FPSCounter(sf::Vector2i position, long refreshRate) :
    _refreshRate(refreshRate),
    _count(0),
    _accumulator(),
    _lastCall(),
    _lastUpdate()
{
    setFont(Font::defaultFont);
    setString(">9000");
    setFillColor(sf::Color::White);
    setOutlineColor(sf::Color::Black);
    setOutlineThickness(1);
    setCharacterSize(30);
    setPosition(position.x, position.y);
}

void FPSCounter::update() {
    _count++;

    auto currentTime    = std::chrono::high_resolution_clock::now();
    auto callTimeDiff   = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _lastCall);
    auto updateTimeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _lastUpdate);

    _accumulator += callTimeDiff;

    if (updateTimeDiff > std::chrono::milliseconds(_refreshRate)) {
        int fps = 1000.f / ((float)_accumulator.count() / _count);
        setString("FPS: " + Utils::toString(fps));

        _count = 0;
        _accumulator = _accumulator.zero();
        _lastUpdate = currentTime;
    }

    _lastCall = currentTime;
}

void FPSCounter::render(RenderEngine& e, sf::RenderWindow& w) {
    if (DebugOptions::showFpsCounter()) {
        w.pushGLStates();
        w.draw(*this);
        w.popGLStates();
    }
}

long FPSCounter::getRefreshRate() {
    return _refreshRate;
}

void FPSCounter::setRefreshRate(long r) {
    _refreshRate = r;
}
