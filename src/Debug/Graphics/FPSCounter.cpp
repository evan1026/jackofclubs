#include <SFML/Graphics.hpp>

#include "Debug/DebugOptions.h"
#include "Debug/Graphics/FPSCounter.h"
#include "Utils/Font.h"
#include "Utils/Utils.h"

/*
 * Initialize a new FPSCounter. Initializes a new sf::Text with a white fill color,
 * a black outline color, and an outline thickness of 1, which is located at the
 * provided position.
 *
 * position    - the upper left corner position to render the underlying sf::Text
 * refreshRate - how often (in milliseconds) to update the text
 */
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

/*
 * Updates the FPSCounter's internal states. This is meant to be called once every
 * frame. It will:
 *     1) Increase the count of how many frames have gone by since the last time
 *        the text was updated
 *     2) Update the text if the specified amount of time has gone by (see constructor)
 */
void FPSCounter::update() {
    _count++;

    auto currentTime    = std::chrono::high_resolution_clock::now();
    auto callTimeDiff   = std::chrono::duration_cast<Milliseconds>(currentTime - _lastCall);
    auto updateTimeDiff = std::chrono::duration_cast<Milliseconds>(currentTime - _lastUpdate);

    _accumulator += callTimeDiff;

    if (updateTimeDiff > Milliseconds(_refreshRate)) {

        // fps = (frames / milli) * (millis / second)
        int fps = ((float)_count / (float)_accumulator.count()) * (1000.f);

        setString("FPS: " + Utils::toString(fps));

        // reset everything
        _count = 0;
        _accumulator = _accumulator.zero();
        _lastUpdate = currentTime;
    }

    _lastCall = currentTime;
}

/*
 * Renders the FPSCounter to the screen
 * Overrides IRenderable::render()
 *
 * See super method for more documentation
 */
void FPSCounter::render(RenderEngine& e, sf::RenderWindow& w) {
    if (DebugOptions::showFpsCounter()) {
        w.pushGLStates();
        w.draw(*this);
        w.popGLStates();
    }
}

/*
 * Returns the counter's refresh rate, i.e., how often it will update
 * the text, in milliseconds
 */
long FPSCounter::getRefreshRate() {
    return _refreshRate;
}

/*
 * Sets how often to update the text
 *
 * r - refresh rate, in milliseconds
 */
void FPSCounter::setRefreshRate(long r) {
    _refreshRate = r;
}
