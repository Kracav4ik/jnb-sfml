#ifndef JNB_SFML_RABBIT_H
#define JNB_SFML_RABBIT_H

#include <SFML/Graphics.hpp>
#include "params.h"
#include "level.h"
#include "render.h"
#include "frame_animation.h"

using namespace sf;

const Vector2f RABBIT_SIZE(24, 32);

struct Rabbit : RenderableAutoregister {
    Params params;
    const Level& _level;
    FrameAnim _anim_rabbit;

    Rabbit(const Level& level);

    void draw(RenderWindow& window) const;

    void next_step(float(elapsed));

    void jump();

    void accel_left();

    void accel_right();

    bool can_jump() const;
};

#endif //JNB_SFML_RABBIT_H
