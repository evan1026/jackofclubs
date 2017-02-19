#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Window/Event.hpp>

#include "Rendering/IRenderable.h"
#include "Utils/Events/IEventHandler.h"

/*! \callergraph
 * Base class for screens. Just defines some functions that need to
 * be overridden. (Not shown: IRenderable::render() also must be
 * overridden).
 */
class Screen : public IRenderable, public IEventHandler {
    public:
        /*! \callergraph
         * Overridable function that is called once per frame to tick
         * the screen
         */
        virtual void tick() = 0;
};

#endif
