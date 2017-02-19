#ifndef I_MOUSE_EVENT_HANDLER_H
#define I_MOUSE_EVENT_HANDLER_H

#include <SFML/Window/Event.hpp>

/*! Interface for classes that will deal with mouse events */
class IMouseEventHandler {
    public:

        /*! \callergraph
         *
         * Called when a mouse button has been pushed down. Returns true if the event has been handled or false otherwise.
         *
         * \p e - The mouse event to handle
         */
        virtual bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& e) = 0;

        /*! \callergraph
         *
         * Called when a mouse button has been pushed released. Returns true if the event has been handled or false otherwise.
         *
         * \p e - The mouse event to handle
         */
        virtual bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& e) = 0;

        /*! \callergraph
         *
         * Called when the mouse has moved. Returns true if the event has been handled or false otherwise.
         *
         * \p e - The mouse event to handle
         */
        virtual bool handleMouseMoved(const sf::Event::MouseMoveEvent& e) = 0;
};

#endif
