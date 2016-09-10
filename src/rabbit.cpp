#include <SFML/Graphics/RectangleShape.hpp>
#include "rabbit.h"
#include "utils.h"

void Rabbit::draw(RenderWindow& window) const {
    Color color = Color(222, 190, 140);
    if (can_jump()) {
        color = Color(128, 255, 128);
    }
    draw_rect(window, params.position(), RABBIT_SIZE, color);
    _anim_rabbit.draw(window);
}

void Rabbit::jump() {
    if (params._speed.y >= -50) {
        params._speed.y = -700;
    }
}

void Rabbit::accel_left() {
    params._speed.x = -100;
}

void Rabbit::accel_right() {
    params._speed.x = 100;
}

Rabbit::Rabbit(const Level& level,const RabbitKeybind& keybind)
    : params(Vector2f(400, 300), Vector2f(), RABBIT_SIZE), _level(level),
        _anim_rabbit(FilePath("anims\\r1_run_right.txt"),0,0,2),
        _keybind(keybind)
{
}

bool Rabbit::can_jump() const {
    return _level.block_under_rect(params.get_rect());
}

void Rabbit::next_step(float elapsed) {
    if (_keybind.is_jump_presset() && can_jump()) {
        jump();
    }
    if (_keybind.is_left_presset()) {
        accel_left();
    }
    if (_keybind.is_right_presset()) {
        accel_right();
    }

    _anim_rabbit.set_pos(params._position.x, params._position.y);
    _anim_rabbit.step(elapsed);
}

void Rabbit::right_pressed(bool is_pressed) {
    if (is_pressed){
        accel_right();
    }
}

void Rabbit::left_pressed(bool is_pressed) {
    if (is_pressed){
        accel_left();
    }
}

void Rabbit::jump_pressed(bool is_pressed) {
    if (is_pressed && can_jump()){
        jump();
    }
}


bool RabbitKeybind::is_jump_presset() {
    return Keyboard::isKeyPressed(jump_key);

}

bool RabbitKeybind::is_left_presset() {
    return Keyboard::isKeyPressed(left_key);
}

bool RabbitKeybind::is_right_presset() {
    return Keyboard::isKeyPressed(right_key);
}

RabbitKeybind::RabbitKeybind(Keyboard::Key jump_key, Keyboard::Key left_key, Keyboard::Key right_key) :
        jump_key(jump_key),
        left_key(left_key),
        right_key(right_key)
{}
