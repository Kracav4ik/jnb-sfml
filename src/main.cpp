#include <SFML/Graphics.hpp>
#include <vector>

#include "level.h"
#include "rabbit.h"
#include "debug.h"
#include "utils.h"
#include "render.h"
#include "animation.h"
#include "texture_manager.h"
#include "frame_animation.h"

using namespace sf;

int GRAVITY = 1500;

struct HitInfo {
    HitInfo() : hit_x(false), hit_y(false) {}

    bool hit_x;
    bool hit_y;
};

Vector2f next_pos_hit_x(const FloatRect &rect, const Vector2f &old_pos, Vector2f next_pos, const Vector2f &delta_pos, bool& use) {
    if (old_pos.x + CELL_SIZE.x <= rect.left) {
        next_pos.x = rect.left - CELL_SIZE.x;
        float new_y = (next_pos.x - old_pos.x) * delta_pos.y / delta_pos.x;
        next_pos.y = old_pos.y + new_y;
//        log("iti\nmultiply = %f, delta_pos.y = %f, delta_pos.x = %f\nnext_pos.x = %f, next_pos.y = %f",next_pos.x - rect.left + CELL_SIZE.x, delta_pos.y, delta_pos.x, next_pos.x,next_pos.y);
    } else if (old_pos.x >= rect.left + CELL_SIZE.x) {
        next_pos.x = rect.left + CELL_SIZE.x;
        float new_y = (next_pos.x - old_pos.x) * delta_pos.y / delta_pos.x;
        next_pos.y = old_pos.y + new_y;
//        log("ni\nmultiply = %f, delta_pos.y = %f, delta_pos.x = %f\nnext_pos.x = %f, next_pos.y = %f",next_pos.x - rect.left + CELL_SIZE.x, delta_pos.y, delta_pos.x, next_pos.x,next_pos.y);
    } else {
        use = false;
    }
    return next_pos;
}

Vector2f next_pos_hit_y(const FloatRect &rect, const Vector2f &old_pos, Vector2f next_pos, const Vector2f &delta_pos, bool& use) {
    if (old_pos.y + CELL_SIZE.y <= rect.top) {
        next_pos.y = rect.top - CELL_SIZE.y;
        float new_x = (next_pos.y - old_pos.y) * delta_pos.x / delta_pos.y;
        next_pos.x = old_pos.x + new_x;
//        log("san\nmultiply = %f, delta_pos.y = %f, delta_pos.x = %f\nnext_pos.x = %f, next_pos.y = %f",rect.top - next_pos.y + CELL_SIZE.y, delta_pos.y, delta_pos.x, next_pos.x,next_pos.y);
    } else if(old_pos.y >= rect.top + CELL_SIZE.y) {
        next_pos.y = rect.top + CELL_SIZE.y;
        float new_x = (next_pos.y - old_pos.y) * delta_pos.x / delta_pos.y;
        next_pos.x = old_pos.x + new_x;
//        log("yo\nmultiply = %f, delta_pos.y = %f, delta_pos.x = %f\nnext_pos.x = %f, next_pos.y = %f",next_pos.y - rect.top + CELL_SIZE.y, delta_pos.y, delta_pos.x, next_pos.x,next_pos.y);
    } else {
        use = false;
    }
    return next_pos;
}

float len2(const Vector2f& v) {
    return v.x*v.x + v.y*v.y;
}

Vector2f collide_rect_pieces(RenderWindow& window, const Level& level, const Vector2f& old_pos, const FloatRect& obj_rect, HitInfo& hit_info) {
    Vector2f next_pos(obj_rect.left, obj_rect.top);
//    draw_line(window, old_pos, next_pos, Color(255, 0, 0));
    std::vector<FloatRect> collided;
    static Color colors[] = {
            {255, 105, 0},
            {105, 255, 0},
            {0,  105, 255},
    };
    if (level.intersects(obj_rect, &collided)) {
        for (int i = 0; i < collided.size(); i += 1) {
            Vector2f delta_pos = next_pos - old_pos;
            FloatRect rect = collided[i];
            FloatRect intersection;
            FloatRect next_obj_rect(next_pos, CELL_SIZE);
            if (!rect.intersects(next_obj_rect, intersection)) {
                continue;
            }
//            draw_rect(window, Vector2f(rect.left, rect.top), CELL_SIZE, colors[i], false);
            bool use_x = true;
            bool use_y = true;
            Vector2f next_pos_x = next_pos_hit_x(rect, old_pos, next_pos, delta_pos, use_x);
            Vector2f next_pos_y = next_pos_hit_y(rect, old_pos, next_pos, delta_pos, use_y);
            if (!use_x) {
                next_pos = next_pos_y;
                hit_info.hit_y = true;
            } else if (!use_y) {
                next_pos = next_pos_x;
                hit_info.hit_x = true;
            } else if (len2(next_pos_x - next_pos) < len2(next_pos_y - next_pos)) {
                next_pos = next_pos_x;
                hit_info.hit_x = true;
            } else {
                next_pos = next_pos_y;
                hit_info.hit_y = true;
            }
//            log("bbl_next(%f, %f), bbl_rect(%f, %f)\n", next_pos.x, next_pos.y, rect.left, rect.top);
//            draw_rect(window, next_pos, CELL_SIZE/5.f, colors[i], false);
        }
    }
//    draw_line(window, old_pos, next_pos, Color(255, 0, 0));
    return next_pos;
}

Vector2f collide_rect(RenderWindow& window, const Level& level, const Vector2f& old_pos, const FloatRect& obj_rect, HitInfo& hit_info){
    Vector2f next_pos(obj_rect.left, obj_rect.top);
    Vector2f delta_pos = next_pos - old_pos;
    float len_delta = sqrtf(len2(delta_pos));
    Vector2f result = next_pos;
    int count = int(len_delta / CELL_SIZE.x) + 1;
    Vector2f norm_delta = delta_pos / float(count);
    Vector2f new_old_pos = old_pos;
//    log("count is %d, len is %f\n", count, len_delta);
    if (len_delta < 2) {
//        log("fuck you\n");
    }
    for(int _ = 0; _ < count; _ += 1 ){
        result = collide_rect_pieces(window, level, new_old_pos, FloatRect(new_old_pos + norm_delta, CELL_SIZE), hit_info);
        new_old_pos += norm_delta;
        if (hit_info.hit_x || hit_info.hit_y){
//            log("break!\n");
            break;
        }
    }
    return result;
}

struct HardcodedRabbit : Animation, RenderableAutoregister {
    int _x;
    int _y;
    bool _upper;

    HardcodedRabbit(int x, int y, bool upper) : _x(x), _y(y), _upper(upper) {}

    virtual void draw(RenderWindow &window) const {
        Texture& tex = TextureManager::inst().get_texture("..\\data\\run_anim.png");
        int w = 16;
        int h = 20;
        int dx = w * int(time*4);
        int dy = _upper ? 0 : 32;
        Sprite sprite;
        sprite.setTexture(tex);
        sprite.setTextureRect(IntRect(dx, dy, w, h));
        sprite.setPosition(_x, _y);
        sprite.setScale(4, 4);
        window.draw(sprite);
    }
};

int main() {
    RenderWindow  window(VideoMode(800, 512), "My window");
    RenderManager& render_manager = RenderManager::inst();

    Level level;
    level.print();
    Rabbit rabbit(level);
    HardcodedRabbit r1(50, 50, true);
    HardcodedRabbit r2(600, 50, false);
    Vector2f gravity(0, GRAVITY);
    FrameAnim frame_anim(FilePath("anims\\r1_run_right.txt"), 100, 100, 10);

    window.setPosition(Vector2i(45, 50));
    window.setKeyRepeatEnabled(false);

    Clock clock;
    const float MAX_FPS = 30;

    // debug for collision
    bool mouse_debug = false;
    Vector2f mouse_click;

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
            } else if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    mouse_debug = true;
                    mouse_click.x = event.mouseButton.x;
                    mouse_click.y = event.mouseButton.y;
                } else if (event.mouseButton.button == Mouse::Right) {
                    mouse_debug = false;
                }
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::W) && rabbit.can_jump()) {
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
        next_params._position = collide_rect(window, level, rabbit.params.position(), next_params.get_rect(), hit_info);
        if (hit_info.hit_x) {
            next_params._speed.x = 0;
        }
        if (hit_info.hit_y) {
            next_params._speed.y = 0;
        }

        rabbit.params = next_params;

        // debug for collision
        Vector2f mouse_pos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
        HitInfo mouse_hit;
        Vector2f fixed_pos;
        if (mouse_debug) {
            fixed_pos = collide_rect(window, level, mouse_click, FloatRect(mouse_pos, CELL_SIZE), mouse_hit);
        }
        r1.step(elapsed);
        r2.step(elapsed);
        frame_anim.step(elapsed);

        // clear the window with black color
        window.clear(Color::Black);

        // draw everything here...
        render_manager.render(window);

        // debug for collision
        if (mouse_debug) {
            draw_rect(window, mouse_click, CELL_SIZE, Color(128, 128, 128), false);
            draw_rect(window, mouse_pos, CELL_SIZE, Color(0, 0, 0), false);
            draw_rect(window, fixed_pos, CELL_SIZE, Color(255, 0, 255), false);
        }

        // end the current frame
        window.display();

        float sleep_time = 1/MAX_FPS - elapsed;
        if (sleep_time > 0) {
            sleep(microseconds((Int64)(sleep_time * 1e6)));
        }
    }

    return 0;
}
