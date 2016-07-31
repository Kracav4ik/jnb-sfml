#include "level.h"
#include "utils.h"
using namespace sf;

Level::Level() {
    for (int x = 0; x < SIZE_X; x += 1) {
        for (int y = 0; y < SIZE_Y; y += 1) {
            if (x == 0 || y == 0 || x == SIZE_X - 1 || y == SIZE_Y - 1) {
                geometry[x][y] = '#';
            } else {
                geometry[x][y] = ' ';
            }
        }
    }

    for (int i = 1; i < 4; i += 1) {
        geometry[SIZE_X / 2][SIZE_Y - i] = '#';
    }

    for (int i = - 3; i < 4; i += 1) {
        geometry[SIZE_X / 2 + i][SIZE_Y - 4] = '#';
    }
}

void Level::draw(RenderWindow& window) {
    draw_rect(window, Vector2f(), Vector2f(800, 512), Color(0, 109, 214));

    for (int x = 0; x < SIZE_X; x += 1) {
        for (int y = 0; y < SIZE_Y; y += 1) {
            char c = geometry[x][y];
            if (c == '#') {
                Vector2f d1(1, 1);
                Vector2f pos(CELL_SIZE.x * x, CELL_SIZE.y * y);
                draw_rect(window, pos,      CELL_SIZE,          Color(164, 81, 0));
                draw_rect(window, pos + d1, CELL_SIZE - d1,     Color(82, 16, 0));
                draw_rect(window, pos + d1, CELL_SIZE - 2.f*d1, Color(123, 45, 0));
            }
        }
    }
}

void Level::print() {
    for (int y = 0; y < SIZE_Y; y += 1) {
        for (int x = 0; x < SIZE_X; x += 1) {
            printf("%c", geometry[x][y]);
        }
        printf("\n");
    }
}

bool Level::intersects(const FloatRect& rect, std::vector<FloatRect>& vector) {
    bool b = false;
    for (int x = 0; x < SIZE_X; x += 1) {
        for (int y = 0; y < SIZE_Y; y += 1) {
            char c = geometry[x][y];
            if (c == '#') {
                FloatRect cell_rect(Vector2f(CELL_SIZE.x * x, CELL_SIZE.y * y), CELL_SIZE);
                if (cell_rect.intersects(rect)) {
                    vector.push_back(cell_rect);
                    b = true;
                }
            }
        }
    }
    return b;
}
