#include <SFML/Graphics/RectangleShape.hpp>
#include "rabbit.h"
#include "utils.h"

void Rabbit::draw(sf::RenderWindow& window) {
    draw_rect(window, position, sf::Vector2f(32, 32), sf::Color(222, 190, 140));
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
    : position(400, 300)
{
}
