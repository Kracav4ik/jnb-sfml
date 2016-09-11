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

    bool is_jump_pressed();
    bool is_left_pressed();
    bool is_right_pressed();
};

struct Rabbit : RenderableAutoregister {
    Params params;
    const Level& _level;
    FrameAnim _anim_rabbit_stand_left ;
    FrameAnim _anim_rabbit_stand_right;
    FrameAnim _anim_rabbit_run_left;
    FrameAnim _anim_rabbit_run_right;
    FrameAnim* _current_anim;
    RabbitKeybind _keybind;
    bool _is_left;

    Rabbit(const Level& level, const RabbitKeybind& _keybind);

    void draw(RenderWindow& window) const;

    void next_step(float(elapsed));

    void jump();

    void accel_left();

    void accel_right();

    bool can_jump() const;
};

#endif //JNB_SFML_RABBIT_H
