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

void draw_line(RenderWindow& window, Vector2f pos1, Vector2f pos2, Color color) {
    VertexArray line(Lines, 2);
    line[0] = pos1;
    line[0].color = color;
    line[1] = pos2;
    line[1].color = color;
    window.draw(line);
}
