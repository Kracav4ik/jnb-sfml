#ifndef JNB_SFML_RABBIT_H
#define JNB_SFML_RABBIT_H

#include <SFML/Graphics.hpp>

const sf::Vector2f RABBIT_SIZE(32, 32);

struct Rabbit {
    sf::Vector2f position;
    sf::Vector2f speed;

    Rabbit();

    void draw(sf::RenderWindow& window);

    void jump();

    void accel_left();

    void accel_right();

    sf::FloatRect get_rect() const;
};

#endif //JNB_SFML_RABBIT_H
