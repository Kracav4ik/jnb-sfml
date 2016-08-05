#ifndef JNB_SFML_FRAME_ANIMATION_H
#define JNB_SFML_FRAME_ANIMATION_H

#include <vector>
#include "animation.h"
#include "file_path.h"
#include "render.h"
#include <SFML/Graphics.hpp>

using namespace sf;

struct Frame {
    int _x;
    int _y;
    int _w;
    int _h;
    int _dx;
    int _dy;

    Frame(int x, int y, int w, int h, int dx, int dy);
};


struct FrameAnim : Animation, RenderableAutoregister {
    float _x;
    float _y;
    float _scale;
    std::vector<Frame> _frames;
    Texture* _tex;

    FrameAnim(const FilePath& path, int x, int y, float scale);

    virtual void draw(RenderWindow& window) const override;
};

#endif //JNB_SFML_FRAME_ANIMATION_H
