#include "frame_animation.h"
#include "utils.h"
#include "texture_manager.h"
#include "debug.h"

FrameAnim::FrameAnim(const FilePath& path, int x, int y, float scale) : _x(x), _y(y), _scale(scale), _tex(NULL), animInfo(path) {
    _tex = &TextureManager::inst().get_texture(animInfo._tex_name);
}

void FrameAnim::draw(RenderWindow& window) const {
    const Frame& frame = animInfo._frames[int(time * animInfo._frames.size())];

    Sprite sprite;
    sprite.setTexture(*_tex);
    sprite.setTextureRect(IntRect(frame._x, frame._y, frame._w, frame._h));
    sprite.setPosition(_x - frame._dx * _scale, _y - frame._dy * _scale);
    sprite.setScale(_scale, _scale);
    window.draw(sprite);
}


Frame::Frame(int x, int y, int w, int h, int dx, int dy) : _x(x), _y(y), _w(w), _h(h), _dx(dx), _dy(dy) {}

AnimInfo::AnimInfo() {}

AnimInfo::AnimInfo(const FilePath& path) {
    load(path);
}

void AnimInfo::load(const FilePath& path) {
    TextFile file(path);
    if (!file.open()) {
        printf("cannot open \"%s\"\n",path.str());
        return;
    }
    _tex_name = strip(file.read_line());
    std::string current_frame = file.read_line();
    _frames.clear();

    while (!current_frame.empty()) {
        std::string result;
        std::vector<int> results;
        for (int i = 0; i < current_frame.size(); i++) {
            char c = current_frame[i];
            if (c == ' ' || c == '\n') {
                results.push_back(atoi(result.c_str()));
                result = "";
                continue;
            }
            result.push_back(c);
        }
        if(!result.empty()) {
            results.push_back(atoi(result.c_str()));
        }
        int xx = results[0];
        int yy = results[1];
        int w = results[2];
        int h = results[3];
        int dx = results[4];
        int dy = results[5];
        _frames.push_back(Frame(xx, yy, w, h, dx, dy));
        current_frame = file.read_line();
    }
}
