#ifndef JNB_SFML_FRAME_ANIMATION_H
#define JNB_SFML_FRAME_ANIMATION_H

#include <vector>
#include "animation.h"
#include "file_path.h"
#include "render.h"
#include <SFML/Graphics.hpp>

using namespace sf;

struct IntPoint2D {
    int x;
    int y;

    IntPoint2D(int x=0, int y=0) : x(x), y(y) {}
};
bool operator==(const IntPoint2D& p1, const IntPoint2D& p2);

#define GETTER_SETTER(NAME, CODE)                       \
    decltype(CODE)  NAME() const { return CODE; }       \
    decltype(CODE)& NAME()       { return CODE; }

struct Frame {
    IntPoint2D pos;
    IntPoint2D size;
    IntPoint2D offset;

    GETTER_SETTER(x, pos.x)
    GETTER_SETTER(y, pos.y)

    GETTER_SETTER(w, size.x)
    GETTER_SETTER(h, size.y)

    GETTER_SETTER(dx, offset.x)
    GETTER_SETTER(dy, offset.y)

    Frame(int x, int y, int w, int h, int dx, int dy);
};
bool operator==(const Frame& f1, const Frame& f2);

struct AnimInfo{
    typedef std::vector<Frame> Frames;
    Frames _frames;
    std::string _tex_name;

    AnimInfo();

    AnimInfo(const FilePath& path);

    void load(const FilePath& path);

    void clear();

    bool empty() const { return _frames.empty(); }

    void save(const FilePath& path);
};

struct FrameAnim : Animation, RenderableAutoregister {
    float _x;
    float _y;
    float _scale;
    AnimInfo animInfo;
    Texture* _tex;

    FrameAnim(const FilePath& path, int x, int y, float scale);

    virtual void draw(RenderWindow& window) const override;
};

#endif //JNB_SFML_FRAME_ANIMATION_H
