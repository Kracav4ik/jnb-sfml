#include <SFML/Graphics/RectangleShape.hpp>
#include "rabbit.h"

void Rabbit::draw(sf::RenderWindow& window) {
    sf::RectangleShape rectangle(sf::Vector2f(32, 32));

    rectangle.setPosition(position);
    rectangle.setFillColor(sf::Color(222, 190, 140));

    window.draw(rectangle);
}

void Rabbit::jump() {
    position.y -= 0.1;
}

void Rabbit::accel_left() {
    position.x -= 0.1;
}

void Rabbit::accel_right() {
    position.x += 0.1;
}

Rabbit::Rabbit()
    : position(400, 300)
{
}
