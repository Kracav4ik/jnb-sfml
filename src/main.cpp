#include <SFML/Graphics.hpp>

#include "level.h"
#include "rabbit.h"

int main() {
    sf::RenderWindow  window(sf::VideoMode(800, 512), "My window");

    Level level;
    Rabbit rabbit;

    window.setPosition(sf::Vector2i(45, 50));
    window.setKeyRepeatEnabled(false);

    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            rabbit.jump();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            rabbit.accel_left();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            rabbit.accel_right();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        level.draw(window);
        rabbit.draw(window);

        // end the current frame
        window.display();
    }

    return 0;
}
