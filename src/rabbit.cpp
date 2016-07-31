#include <SFML/Graphics/RectangleShape.hpp>
#include "rabbit.h"
#include "utils.h"

void Rabbit::draw(RenderWindow& window) const {
    draw_rect(window, params.position(), RABBIT_SIZE, Color(222, 190, 140));
}

void Rabbit::jump() {
    params._speed.y = -200;
}

void Rabbit::accel_left() {
    params._speed.x = -100;
}

void Rabbit::accel_right() {
    params._speed.x = 100;
}

Rabbit::Rabbit()
    : params(Vector2f(400, 300), Vector2f(), RABBIT_SIZE)
{
}
