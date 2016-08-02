#ifndef JNB_SFML_LEVEL_H
#define JNB_SFML_LEVEL_H

#include <SFML/Graphics/Rect.hpp>

namespace sf {
    struct RenderWindow;
}

using namespace sf;

const int SIZE_X = 25;
const int SIZE_Y = 16;
const Vector2f CELL_SIZE(32, 32);

struct Level {
    char geometry[SIZE_X][SIZE_Y];

    Level();

    void draw(RenderWindow& window);

    void print();

    bool intersects(const FloatRect& rect, std::vector<FloatRect>* vector=NULL) const;

    bool block_under_rect(const FloatRect &rect) const;
};

#endif //JNB_SFML_LEVEL_H
