#ifndef JNB_SFML_RENDER_H
#define JNB_SFML_RENDER_H

#include <SFML/Graphics.hpp>
#include <list>

using namespace sf;

struct Renderable {
    virtual void draw(RenderWindow& window) const = 0;
    virtual ~Renderable() {}
};

struct RenderManager {
    std::list<const Renderable*> renderables;

    void add_renderable(const Renderable& renderable);
    void remove_renderable(const Renderable& renderable);

    void render(RenderWindow& window) const;
};


#endif //JNB_SFML_RENDER_H
