#ifndef JNB_SFML_RABBIT_H
#define JNB_SFML_RABBIT_H

#include <SFML/Graphics.hpp>
#include "params.h"

const sf::Vector2f RABBIT_SIZE(32, 32);

struct Rabbit {
    sf::Vector2f position;
    sf::Vector2f speed;
    Params params;

    Rabbit();

    void draw(sf::RenderWindow& window);

    void jump();

    void accel_left();

    void accel_right();

};

#endif //JNB_SFML_RABBIT_H
