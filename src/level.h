#ifndef JNB_SFML_LEVEL_H
#define JNB_SFML_LEVEL_H

namespace sf {
    struct RenderWindow;
}

const int SIZE_X = 25;
const int SIZE_Y = 16;
const int CELL_SIZE = 32;

struct Level {
    char geometry[SIZE_X][SIZE_Y];

    Level();

    void draw(sf::RenderWindow& window);

    void print();
};

#endif //JNB_SFML_LEVEL_H
