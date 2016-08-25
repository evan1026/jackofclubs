#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include <chrono>
#include <SFML/Graphics.hpp>

#include "Rendering/IRenderable.h"

class FPSCounter : public sf::Text, public IRenderable {

    long _refreshRate;
    long _count;
    std::chrono::milliseconds _accumulator;
    std::chrono::high_resolution_clock::time_point _lastCall,
                                                   _lastUpdate;

    public:
        FPSCounter(sf::Vector2i position, long refreshRate = 500); //refresh rate in millis

        // Call every frame. It will count the number of times it was called
        // and update the text every time the refresh time goes by
        void update();

        void render(RenderEngine& e, sf::RenderWindow& w) override;

        long getRefreshRate();
        void setRefreshRate(long r);
};

#endif
