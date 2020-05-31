#pragma once

#include <SFML/Window/Event.hpp>

/*! Interface for classes that will handle resize events */
class IResizeEventHandler {
    public:

        /*! \callergraph
         *
         * Called on a resize. Returns true if the event has been handled or false otherwise.
         * In the vast majority of cases, this should return false, unless for some reason, you
         * don't want to notify anyone else that a resize has occured.
         *
         * \p e - The resize event to handle
         */
        virtual bool handleResize(const sf::Event::SizeEvent& e) = 0;
};
