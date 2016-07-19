#ifndef JNB_SFML_PARAMS_H
#define JNB_SFML_PARAMS_H


#include <SFML/Graphics.hpp>
using namespace sf;
struct Params {
    Vector2f _speed;
    Vector2f _position;
    Vector2f _size;

    Params(Vector2f position=Vector2f(), Vector2f speed=Vector2f(), Vector2f size = Vector2f());

    Vector2f position() const;

    Vector2f speed() const;

    Vector2f size() const;

    FloatRect get_rect() const;

};


#endif //JNB_SFML_PARAMS_H
