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





