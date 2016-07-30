#include "params.h"


Params::Params(const Vector2f& position, const Vector2f& speed, const Vector2f& size)
:_position(position), _speed(speed), _size(size)
{
}

Vector2f Params::speed() const {
    return _speed;
}

Vector2f Params::position() const {
    return _position;
}

Vector2f Params::size() const {
    return _size;
}

FloatRect Params::get_rect() const {
    return FloatRect(position(), size());
}
