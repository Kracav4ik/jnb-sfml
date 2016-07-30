#ifndef JNB_SFML_LEVEL_H
#define JNB_SFML_LEVEL_H

#include <SFML/Graphics/Rect.hpp>

namespace sf {
    struct RenderWindow;
}

const int SIZE_X = 25;
const int SIZE_Y = 16;
const sf::Vector2f CELL_SIZE(32, 32);

struct Level {
    char geometry[SIZE_X][SIZE_Y];

    Level();

    void draw(sf::RenderWindow& window);

    void print();

    bool intersects(const sf::FloatRect& rect, std::vector<sf::FloatRect>& vector);
};

#endif //JNB_SFML_LEVEL_H
