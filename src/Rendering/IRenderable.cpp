#include "Rendering/IRenderable.h"
#include "Rendering/RenderEngine.h"

void IRenderable::addToEngine() {
    RenderEngine::getInst().addRenderable(*this);
}

void IRenderable::removeFromEngine() {
    RenderEngine::getInst().removeRenderable(*this);
}
