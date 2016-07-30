#include <SFML/Graphics.hpp>
#include <vector>

#include "level.h"
#include "rabbit.h"

int main() {
    sf::RenderWindow  window(sf::VideoMode(800, 512), "My window");

    Level level;
    level.print();
    Rabbit rabbit;
    sf::Vector2f gravity(0, 450);

    window.setPosition(sf::Vector2i(45, 50));
    window.setKeyRepeatEnabled(false);

    sf::Clock clock;
    const float MAX_FPS = 10;

    // run the program as long as the window is open
    while (window.isOpen()) {
        sf::Int64 useconds = clock.restart().asMicroseconds();
        float elapsed = useconds * 1e-6f;

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

        // process physics
        rabbit.speed += gravity * 0.5f * elapsed;
        rabbit.position += rabbit.speed * elapsed;
        rabbit.speed += gravity * 0.5f * elapsed;
        std::vector<sf::FloatRect> collided;
        if (level.intersects(rabbit.params.get_rect(), collided)) {

            rabbit.speed = sf::Vector2f();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        level.draw(window);
        rabbit.draw(window);

        // end the current frame
        window.display();

        float sleep_time = 1/MAX_FPS - elapsed;
        if (sleep_time > 0) {
            sf::sleep(sf::microseconds((sf::Int64)(sleep_time * 1e6)));
        }
    }

    return 0;
}
