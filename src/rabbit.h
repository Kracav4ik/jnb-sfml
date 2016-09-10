#ifndef JNB_SFML_RABBIT_H
#define JNB_SFML_RABBIT_H

#include <SFML/Graphics.hpp>

#include "params.h"
#include "level.h"
#include "render.h"
#include "frame_animation.h"

using namespace sf;

const Vector2f RABBIT_SIZE(24, 32);

class RabbitKeybind{
public:
    Keyboard::Key jump_key;
    Keyboard::Key left_key;
    Keyboard::Key right_key;
    RabbitKeybind(Keyboard::Key jump_key, Keyboard::Key left_key, Keyboard::Key right_key);

    bool is_jump_presset();
    bool is_left_presset();
    bool is_right_presset();
};

struct Rabbit : RenderableAutoregister {
    Params params;
    const Level& _level;
    FrameAnim _anim_rabbit;
    RabbitKeybind _keybind;

    Rabbit(const Level& level, const RabbitKeybind& _keybind);

    void draw(RenderWindow& window) const;

    void next_step(float(elapsed));

    void jump();

    void jump_pressed(bool is_pressed);

    void accel_left();

    void left_pressed(bool is_pressed);

    void accel_right();

    void right_pressed(bool is_pressed);

    bool can_jump() const;
};

#endif //JNB_SFML_RABBIT_H
