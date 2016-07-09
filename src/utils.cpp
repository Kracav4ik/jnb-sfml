#include "utils.h"

void draw_rect(sf::RenderWindow& window, sf::Vector2f pos, sf::Vector2f size, sf::Color color) {
    sf::RectangleShape block(size);
    block.setPosition(pos);
    block.setFillColor(color);
    window.draw(block);
}
