#pragma once

#include <SFML/Window/Event.hpp>

/*! Interface for raw event handlers */
class IEventHandler {
    public:

        /*! \callergraph
         *
         * Called to handle an event. Return true if the event is finished being handled
         * or false otherwise.
         *
         * \p e - The event to handle
         */
        virtual bool handleEvent(const sf::Event& e) = 0;
};
