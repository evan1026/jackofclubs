#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include <chrono>
#include <SFML/Graphics.hpp>

#include "Rendering/Renderable.h"

typedef std::chrono::milliseconds Milliseconds;
typedef std::chrono::high_resolution_clock::time_point TimePoint;

/*!
 * This class represents the FPS counter that is displayed in the top
 * left of the screen when F3 is pressed. It will automatically handle
 * figuring out the framerate as long as FPSCounter::update is called every frame.
 * It will also handle updating the text at a shorter refresh rate (default
 * is every half second) to ensure readability.
 */
class FPSCounter : public sf::Text, public Renderable {

    long _refreshRate;         // How often (in millis) to update the text
    long _count;               // Counts how many frames have gone by
    Milliseconds _accumulator; // Counts how much time has gone by
    TimePoint _lastCall,       // Last time update() was called
              _lastUpdate;     // Last time text was updated

    public:
        FPSCounter(sf::Vector2i position, long refreshRate = 500); //refresh rate in millis

        // Call every frame. It will count the number of times it was called
        // and update the text every time the refresh time goes by
        void update();

        // Overrides method from IRenderable
        void render(RenderEngine& e, sf::RenderWindow& w) override;

        long getRefreshRate();
        void setRefreshRate(long r);
};

#endif
