#ifndef JNB_SFML_RABBIT_H
#define JNB_SFML_RABBIT_H

#include <SFML/Graphics.hpp>
#include "params.h"
#include "level.h"
#include "render.h"
#include "frame_animation.h"
#include "hitInfo.h"

extern int GRAVITY;

using namespace sf;

const Vector2f RABBIT_SIZE(24, 32);
Vector2f collide_rect(RenderWindow& window, const Level& level, const Vector2f& old_pos, const FloatRect& obj_rect, HitInfo& hit_info);

class RabbitKeybind{
public:
    Keyboard::Key jump_key;
    Keyboard::Key left_key;
    Keyboard::Key right_key;
    Keyboard::Key respawn_key;

    RabbitKeybind(Keyboard::Key jump_key, Keyboard::Key left_key, Keyboard::Key right_key, Keyboard::Key respawn_key);

    bool is_jump_pressed();
    bool is_left_pressed();
    bool is_right_pressed();
    bool is_respawn_pressed();
};

struct Rabbit : RenderableAutoregister {
public:
    Params params;
    const Level& _level;
    FrameAnim _anim_rabbit_stand_left ;
    FrameAnim _anim_rabbit_stand_right;
    FrameAnim _anim_rabbit_run_left;
    FrameAnim _anim_rabbit_run_right;
    FrameAnim* _current_anim;
    RabbitKeybind _keybind;
    bool _is_left;

    enum Player {
        WHITE = 1, ORANGE, BLACK, BROWN
    };

    Rabbit(const Level& level, const RabbitKeybind& _keybind, Player player);

    void draw(RenderWindow& window) const;

    void process_input();

    void process_physics(float elapsed, RenderWindow& window);

    void jump();

    void die();

    void accel_left();

    void accel_right();

    bool can_jump() const;

    void respawn();

    void spawn_at(const FloatRect& rect);

    float x() const;
    float y() const;
    float vx() const;
    float vy() const;
};

#endif //JNB_SFML_RABBIT_H
