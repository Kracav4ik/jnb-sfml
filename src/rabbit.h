#ifndef JNB_SFML_RABBIT_H
#define JNB_SFML_RABBIT_H

#include <SFML/Graphics.hpp>

struct Rabbit {
    sf::Vector2f position;

    Rabbit();

    void draw(sf::RenderWindow& window);

    void jump();

    void accel_left();

    void accel_right();
};

#endif //JNB_SFML_RABBIT_H
