#include <SFML/Graphics/RectangleShape.hpp>
#include "rabbit.h"
#include "utils.h"

void Rabbit::draw(RenderWindow& window) const {
    Color color = Color(222, 190, 140);
    if (can_jump()) {
        color = Color(128, 255, 128);
    }
    draw_rect(window, params.position(), RABBIT_SIZE, color);
    _current_anim->draw(window);
}

void Rabbit::jump() {
    if (params._speed.y >= -50) {
        params._speed.y = -700;
    }
}

void Rabbit::accel_left() {
    params._speed.x = -200;
}

void Rabbit::accel_right() {
    params._speed.x = 200;
}

Rabbit::Rabbit(const Level& level,const RabbitKeybind& keybind)
    : params(Vector2f(400, 300), Vector2f(), RABBIT_SIZE), _level(level),
        _anim_rabbit_stand_left (FilePath("anims/r1_stand_left.txt"),0,0,2),
        _anim_rabbit_stand_right(FilePath("anims/r1_stand_right.txt"),0,0,2),
        _anim_rabbit_run_left (FilePath("anims/r1_run_left.txt"),0,0,2),
        _anim_rabbit_run_right(FilePath("anims/r1_run_right.txt"),0,0,2),
        _current_anim(&_anim_rabbit_stand_right),
        _keybind(keybind),
        _is_left(false)
{
}

bool Rabbit::can_jump() const {
    return _level.block_under_rect(params.get_rect());
}

void Rabbit::next_step(float elapsed) {
    bool left_or_right_pressed = false;
    if (_keybind.is_jump_pressed() && can_jump()) {
        jump();
    }
    if (_keybind.is_left_pressed()) {
        _current_anim = &_anim_rabbit_run_left;
        accel_left();
        left_or_right_pressed = true;
        _is_left = true;
    }
    if (_keybind.is_right_pressed()) {
        _current_anim = &_anim_rabbit_run_right;
        accel_right();
        left_or_right_pressed = true;
        _is_left = false;
    }
    if(!left_or_right_pressed && can_jump()){
        if(_is_left){
            _current_anim = &_anim_rabbit_stand_left;
        } else{
            _current_anim = &_anim_rabbit_stand_right;
        }
        params._speed.x = 0;
    }
     _current_anim->set_pos(params._position.x, params._position.y);
     _current_anim->step(elapsed);
}

bool RabbitKeybind::is_jump_pressed() {
    return Keyboard::isKeyPressed(jump_key);

}

bool RabbitKeybind::is_left_pressed() {
    return Keyboard::isKeyPressed(left_key);
}

bool RabbitKeybind::is_right_pressed() {
    return Keyboard::isKeyPressed(right_key);
}

RabbitKeybind::RabbitKeybind(Keyboard::Key jump_key, Keyboard::Key left_key, Keyboard::Key right_key) :
        jump_key(jump_key),
        left_key(left_key),
        right_key(right_key)
{}
