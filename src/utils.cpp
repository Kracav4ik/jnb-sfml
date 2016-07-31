#include "utils.h"

void draw_rect(RenderWindow& window, Vector2f pos, Vector2f size, Color color, bool fill) {
    RectangleShape block(size);
    block.setPosition(pos);
    if (fill) {
        block.setFillColor(color);
    } else {
        block.setFillColor(Color::Transparent);
        block.setOutlineColor(color);
        block.setOutlineThickness(1);
    }
    window.draw(block);
}
