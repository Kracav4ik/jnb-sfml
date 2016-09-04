#include "frame_animation.h"
#include "utils.h"
#include "texture_manager.h"
#include "debug.h"

FrameAnim::FrameAnim(const FilePath& path, int x, int y, float scale) :
        _x(x),
        _y(y),
        _scale(scale),
        _tex(NULL),
        animInfo(path) {
    _tex = &TextureManager::inst().get_texture(animInfo._tex_name);
}

void FrameAnim::draw(RenderWindow& window) const {
    const Frame& frame = animInfo._frames[int(time * animInfo._frames.size())];

    Sprite sprite;
    sprite.setTexture(*_tex);
    sprite.setTextureRect(IntRect(frame.x(), frame.y(), frame.w(), frame.h()));
    sprite.setPosition(_x - frame.dx() * _scale, _y - frame.dy() * _scale);
    sprite.setScale(_scale, _scale);
    window.draw(sprite);
}


Frame::Frame(int x, int y, int w, int h, int dx, int dy) :
        pos(x, y),
        size(w, h),
        offset(dx, dy)
{
}

AnimInfo::AnimInfo() {}

AnimInfo::AnimInfo(const FilePath& path) {
    load(path);
}

void AnimInfo::load(const FilePath& path) {
    TextFile file(path);
    if (!file.open()) {
        printf("cannot open for read \"%s\"\n", path.str());
        return;
    }
    clear();

    _tex_name = strip(file.read_line());
    std::string current_frame = file.read_line();

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
        if (!result.empty()) {
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

void AnimInfo::clear() {
    _tex_name = "";
    _frames.clear();
}

void AnimInfo::save(const FilePath& path) {
    FILE* file = fopen(path.str(), "w");
    if (!file) {
        printf("cannot open for write \"%s\"\n", path.str());
        return;
    }
    fprintf(file, "%s", _tex_name.c_str());
    for (int i = 0; i < _frames.size(); i++) {
        Frame& frame = _frames[i];
        fprintf(file, "\n%d %d %d %d %d %d", frame.x(), frame.y(), frame.w(), frame.h(), frame.dx(), frame.dy());
    }
    fclose(file);
}

bool operator==(const IntPoint2D& p1, const IntPoint2D& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator==(const Frame& f1, const Frame& f2) {
    return f1.pos == f2.pos && f1.size == f2.size && f1.offset == f2.offset;
}
