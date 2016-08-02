#include <SFML/Graphics/RectangleShape.hpp>
#include "rabbit.h"
#include "utils.h"

void Rabbit::draw(RenderWindow& window) const {
    Color color = Color(222, 190, 140);
    if (can_jump()) {
        color = Color(128, 255, 128);
    }
    draw_rect(window, params.position(), RABBIT_SIZE, color);
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

Rabbit::Rabbit(const Level& level)
    : params(Vector2f(400, 300), Vector2f(), RABBIT_SIZE), _level(level)
{
}

bool Rabbit::can_jump() const {
    return _level.block_under_rect(params.get_rect());
}
