#pragma once

#include "Graphics/Menu/Menu.h"
#include "Rendering/Renderable.h"
#include "Utils/Events/IEventHandler.h"

/*! \callergraph
 * Base class for screens. Just defines some functions that need to
 * be overridden. (Not shown: Renderable::render() also must be
 * overridden).
 */
class Screen : public Renderable, public IEventHandler {
    public:
        /*! \callergraph
         * Overridable function that is called once per frame to tick
         * the screen
         */
        virtual void tick() = 0;
        virtual ~Screen() = default;
        virtual void addMenu(Menu* const m) = 0;
        virtual void removeMenu() = 0;
};
