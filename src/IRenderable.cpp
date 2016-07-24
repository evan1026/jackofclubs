#include "IRenderable.h"
#include "RenderEngine.h"

void IRenderable::addToEngine() {
    RenderEngine::getInst().addRenderable(*this);
}

void IRenderable::removeFromEngine() {
    RenderEngine::getInst().removeRenderable(*this);
}
