#include "render.h"
#include "utils.h"


void RenderManager::add_renderable(const Renderable& renderable) {
    renderables.push_back(&renderable);
}

void RenderManager::remove_renderable(const Renderable& renderable) {
    renderables.remove(&renderable);
}

void RenderManager::render(RenderWindow &window) const {
    FOREACH_C(std::list, const Renderable*, renderables,
        (*value).draw(window);
    )
}

RenderManager::RenderManager() {
}

RenderManager& RenderManager::inst() {
    static RenderManager manager;
    return manager;
}

RenderableAutoregister::RenderableAutoregister() {
    RenderManager::inst().add_renderable(*this);
}

RenderableAutoregister::~RenderableAutoregister() {
    RenderManager::inst().remove_renderable(*this);
}

