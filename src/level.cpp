#include "level.h"
#include "utils.h"

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

void Level::draw(sf::RenderWindow& window) {
    draw_rect(window, sf::Vector2f(), sf::Vector2f(800, 512), sf::Color(0, 109, 214));

    for (int x = 0; x < SIZE_X; x += 1) {
        for (int y = 0; y < SIZE_Y; y += 1) {
            char c = geometry[x][y];
            if (c == '#') {
                draw_rect(window, sf::Vector2f(CELL_SIZE.x * x,     CELL_SIZE.y * y),                  CELL_SIZE,                         sf::Color(164, 81, 0));
                draw_rect(window, sf::Vector2f(CELL_SIZE.x * x + 1, CELL_SIZE.y * y + 1), sf::Vector2f(CELL_SIZE.x - 1, CELL_SIZE.y - 1), sf::Color(82, 16, 0));
                draw_rect(window, sf::Vector2f(CELL_SIZE.x * x + 1, CELL_SIZE.y * y + 1), sf::Vector2f(CELL_SIZE.x - 2, CELL_SIZE.y - 2), sf::Color(123, 45, 0));
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

bool Level::intersects(const sf::FloatRect& rect, std::vector<sf::FloatRect>& vector) {
    bool b = false;
    for (int x = 0; x < SIZE_X; x += 1) {
        for (int y = 0; y < SIZE_Y; y += 1) {
            char c = geometry[x][y];
            if (c == '#') {
                sf::FloatRect cell_rect(sf::Vector2f(CELL_SIZE.x * x, CELL_SIZE.y * y), CELL_SIZE);
                if (cell_rect.intersects(rect)) {
                    vector.push_back(rect);
                    b = true;
                }
            }
        }
    }
    return b;
}
