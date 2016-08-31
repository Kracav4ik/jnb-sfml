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

struct Frame {
    IntPoint2D pos;
    IntPoint2D size;
    IntPoint2D offset;

    int x() const { return pos.x; }
    int y() const { return pos.y; }

    int w() const { return size.x; }
    int h() const { return size.y; }

    int  dx() const { return offset.x; }
    int& dx()       { return offset.x; }
    int  dy() const { return offset.y; }
    int& dy()       { return offset.y; }

    Frame(int x, int y, int w, int h, int dx, int dy);
};

struct AnimInfo{
    std::vector<Frame> _frames;
    std::string _tex_name;

    AnimInfo();

    AnimInfo(const FilePath& path);

    void load(const FilePath& path);

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
