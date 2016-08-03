#ifndef JNB_SFML_RABBIT_H
#define JNB_SFML_RABBIT_H

#include <SFML/Graphics.hpp>
#include "params.h"
#include "level.h"
#include "render.h"

using namespace sf;

const Vector2f RABBIT_SIZE(32, 32);

struct Rabbit : Renderable {
    Params params;
    const Level& _level;

    Rabbit(const Level& level);

    void draw(RenderWindow& window) const;

    void jump();

    void accel_left();

    void accel_right();

    bool can_jump() const;
};

#endif //JNB_SFML_RABBIT_H
