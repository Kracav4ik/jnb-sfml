#include <SFML/Graphics.hpp>
#include <vector>

#include "level.h"
#include "rabbit.h"
#include "debug.h"

using namespace sf;

int GRAVITY = 350;

int main() {
    RenderWindow  window(VideoMode(800, 512), "My window");

    Level level;
    level.print();
    Rabbit rabbit;
    Vector2f gravity(0, GRAVITY);

    window.setPosition(Vector2i(45, 50));
    window.setKeyRepeatEnabled(false);

    Clock clock;
    const float MAX_FPS = 30;

    // run the program as long as the window is open
    while (window.isOpen()) {
        Int64 useconds = clock.restart().asMicroseconds();
        float elapsed = useconds * 1e-6f;

        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    window.close();
                }
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            rabbit.jump();
        }
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            rabbit.accel_left();
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            rabbit.accel_right();
        }

        // process physics
        Params next_params = rabbit.params;
        next_params._speed += gravity * 0.5f * elapsed;
        next_params._position += next_params._speed * elapsed;
        next_params._speed += gravity * 0.5f * elapsed;
        std::vector<FloatRect> collised;
        if (level.intersects(next_params.get_rect(), collised)) {
            for (int i = 0; i < collised.size(); i += 1) {
                FloatRect rect = collised[i];
                if (fabsf(next_params.position().x - rect.left) >= fabsf(next_params.position().y - rect.top)) {
                    next_params._speed.x = 0;
                    if (CELL_SIZE.x > next_params.position().x - rect.left + CELL_SIZE.x > 0) {
                        next_params._position.x = rect.left - CELL_SIZE.x;
                        log("iti\n");
                    } else {
                        next_params._position.x = rect.left + CELL_SIZE.x;
                        log("ni\n");
                    }
                } else {
                    next_params._speed.y = 0;
                    if (CELL_SIZE.y > next_params.position().y - rect.top + CELL_SIZE.y > 0) {
                        next_params._position.y = rect.top - CELL_SIZE.y;
                        log("san\n");
                    } else {
                        next_params._position.y = rect.top + CELL_SIZE.y;
                        log("yo\n");
                    }
                }
                log("bbl_next(%f, %f), bbl_rect(%f, %f)\n", next_params.position().x, next_params.position().y,
                       rect.left, rect.top);
            }
        }
        rabbit.params = next_params;

        // clear the window with black color
        window.clear(Color::Black);

        // draw everything here...
        level.draw(window);
        rabbit.draw(window);

        // end the current frame
        window.display();

        float sleep_time = 1/MAX_FPS - elapsed;
        if (sleep_time > 0) {
            sleep(microseconds((Int64)(sleep_time * 1e6)));
        }
    }

    return 0;
}
