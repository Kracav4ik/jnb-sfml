#include <SFML/Graphics/RectangleShape.hpp>
#include "rabbit.h"
#include "utils.h"

void Rabbit::draw(sf::RenderWindow& window) {
    draw_rect(window, position, RABBIT_SIZE, sf::Color(222, 190, 140));
}

void Rabbit::jump() {
    position.y -= 0.3;
}

void Rabbit::accel_left() {
    position.x -= 0.3;
}

void Rabbit::accel_right() {
    position.x += 0.3;
}

Rabbit::Rabbit()
    : position(400, 300), params(speed, position, RABBIT_SIZE)
{
}
