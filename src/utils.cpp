#include "utils.h"

void draw_rect(RenderWindow& window, Vector2f pos, Vector2f size, Color color) {
    RectangleShape block(size);
    block.setPosition(pos);
    block.setFillColor(color);
    window.draw(block);
}
