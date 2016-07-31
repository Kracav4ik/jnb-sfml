#include <SFML/Graphics.hpp>
#include <vector>

#include "level.h"
#include "rabbit.h"
#include "debug.h"

using namespace sf;

int GRAVITY = 1500;

struct HitInfo {
    HitInfo() : hit_x(false), hit_y(false) {}

    bool hit_x;
    bool hit_y;
};

Vector2f collide_rect(const Level& level, const Vector2f& old_pos, const FloatRect& obj_rect, HitInfo& hit_info) {
    Vector2f next_pos(obj_rect.left, obj_rect.top);
    std::vector<FloatRect> collided;
    if (level.intersects(obj_rect, collided)) {
        for (int i = 0; i < collided.size(); i += 1) {
            FloatRect rect = collided[i];
            if (fabsf(obj_rect.left - rect.left) >= fabsf(obj_rect.top - rect.top)) {
                hit_info.hit_x = true;
                if (CELL_SIZE.x > obj_rect.left - rect.left + CELL_SIZE.x) {
                    next_pos.x = rect.left - CELL_SIZE.x;
                    log("iti\n");
                } else {
                    next_pos.x = rect.left + CELL_SIZE.x;
                    log("ni\n");
                }
            } else {
                hit_info.hit_y = true;
                if (CELL_SIZE.y > obj_rect.top - rect.top + CELL_SIZE.y) {
                    next_pos.y = rect.top - CELL_SIZE.y;
                    log("san\n");
                } else {
                    next_pos.y = rect.top + CELL_SIZE.y;
                    log("yo\n");
                }
            }
            log("bbl_next(%f, %f), bbl_rect(%f, %f)\n", next_pos.x, next_pos.y, rect.left, rect.top);
        }
    }
    return next_pos;
}

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

        HitInfo hit_info;
        next_params._position = collide_rect(level, rabbit.params.position(), next_params.get_rect(), hit_info);
        if (hit_info.hit_x) {
            next_params._speed.x = 0;
        }
        if (hit_info.hit_y) {
            next_params._speed.y = 0;
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
