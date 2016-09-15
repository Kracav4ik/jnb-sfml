#include <SFML/Graphics/RectangleShape.hpp>
#include "rabbit.h"
#include "utils.h"
#include "debug.h"

int GRAVITY = 1500;

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

std::string get_prefix(Rabbit::Player player){
    switch (player) {
        case Rabbit::WHITE:
            return "r1";
        case Rabbit::ORANGE:
            return "r2";
        case Rabbit::BLACK:
            return "r3";
        case Rabbit::BROWN:
            return "r4";
        default:
            log("error: unknown player %d\n", player);
            return "";
    }
}

Rabbit::Rabbit(const Level& level,const RabbitKeybind& keybind, Player player)
    : params(Vector2f(400, 300), Vector2f(), RABBIT_SIZE), _level(level),
        _anim_rabbit_stand_left (FilePath("anims/" + get_prefix(player) + "_stand_left.txt"),0,0,2),
        _anim_rabbit_stand_right(FilePath("anims/" + get_prefix(player) + "_stand_right.txt"),0,0,2),
        _anim_rabbit_run_left (FilePath("anims/" + get_prefix(player) + "_run_left.txt"),0,0,2),
        _anim_rabbit_run_right(FilePath("anims/" + get_prefix(player) + "_run_right.txt"),0,0,2),
        _current_anim(&_anim_rabbit_stand_right),
        _keybind(keybind),
        _is_left(false)
{
}

bool Rabbit::can_jump() const {
    return _level.block_under_rect(params.get_rect());
}

void Rabbit::process_input(const Level& level) {
    bool left_or_right_pressed = false;
    if (_keybind.is_respawn_pressed()) {
        spawn_at(level.get_spawn_rect());
    }
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

}

void Rabbit::process_physics(float elapsed, const Level& level, RenderWindow& window) {
    Vector2f gravity(0, GRAVITY);
    Params next_params = params;
    next_params._speed    += gravity * 0.5f     * elapsed;
    next_params._position += next_params._speed * elapsed;
    next_params._speed    += gravity * 0.5f     * elapsed;

    HitInfo hit_info;
    Vector2f fixed_position = collide_rect(window, level, params.position(), next_params.get_rect(), hit_info);
    if(params.position() == fixed_position){
//            log("Same vector from collision: (%f, %f) -> (%f, %f)\n", rabbit.params.position().x, rabbit.params.position().y, next_params.position().x, next_params.position().y);
        Params next_partial = next_params;
        next_partial._position.y = params.position().y;
        hit_info.reset();
        Vector2f move_by_x = collide_rect(window, level, params.position(), next_partial.get_rect(), hit_info);
        if (params.position() == move_by_x){
            if (next_partial.position().x != params.position().x) {
//                    log("  Cannot move by x: (%f, %f) -> (%f, %f)\n", rabbit.params.position().x, rabbit.params.position().y, next_partial.position().x, next_partial.position().y);
            }
            next_partial = next_params;
            next_partial._position.x = params.position().x;
            hit_info.reset();
            fixed_position = collide_rect(window, level, params.position(), next_partial.get_rect(), hit_info);
            hit_info.hit_x = true;
        } else {
            hit_info.hit_y = true;
            fixed_position = move_by_x;
        }
//            log("\n");
    }

    next_params._position = fixed_position;

    if (hit_info.hit_x) {
//            log("HIT X (%f, %f) -> (%f, %f)\n", rabbit.params.position().x, rabbit.params.position().y, next_params.position().x, next_params.position().y);
        next_params._speed.x = 0;
    }
    if (hit_info.hit_y) {
//            log("HIT Y (%f, %f) -> (%f, %f)\n", rabbit.params.position().x, rabbit.params.position().y, next_params.position().x, next_params.position().y);
        next_params._speed.y = 0;
    }

    params = next_params;

    _current_anim->set_pos(params._position.x, params._position.y);
    _current_anim->step(elapsed);
}

void Rabbit::spawn_at(const FloatRect& rect) {
    params._position = Vector2f(rect.left + (CELL_SIZE.x - RABBIT_SIZE.x)/2, rect.top);
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

bool RabbitKeybind::is_respawn_pressed() {
    return Keyboard::isKeyPressed(respawn_key);
}

RabbitKeybind::RabbitKeybind(Keyboard::Key jump_key, Keyboard::Key left_key, Keyboard::Key right_key, Keyboard::Key respawn_key) :
        jump_key(jump_key),
        left_key(left_key),
        right_key(right_key),
        respawn_key(respawn_key)
{}
