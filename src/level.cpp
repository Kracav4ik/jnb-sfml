#include "level.h"

#include <SFML/Graphics.hpp>

void Level::draw(sf::RenderWindow& window) {
    sf::RectangleShape rectangle(sf::Vector2f(800, 512));
    rectangle.setFillColor(sf::Color(0, 109, 214));

    window.draw(rectangle);
}

