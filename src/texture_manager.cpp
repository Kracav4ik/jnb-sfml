
#include "texture_manager.h"


bool load_from_pcx(Texture& texture, const char* name) {
    // TODO
    return false;
}

Texture& TextureManager::get_texture(const char* name) {
    if (texture_map.count(name)){
        return *texture_map[name];
    }
    Texture* texture = new Texture();
    bool loaded = true;
    if (false) { // TODO
        // manually load .pcx
        loaded = load_from_pcx(*texture, name);
    } else {
        loaded = texture->loadFromFile(name);
    }

    if (loaded) {
        texture_map[name] = texture;
        return *texture;
    } else {
        delete texture;
        return empty;
    }
}

TextureManager& TextureManager::inst() {
    static TextureManager manager;
    return manager;
}

TextureManager::TextureManager() {
    Image grid;
    Uint8 pixels[2*2*4] = {
            255,   0,   0, 255,     0, 255,   0, 255,
              0,   0, 255, 255,   128, 128, 128, 255,
    };
    grid.create(2, 2, pixels);
    empty.loadFromImage(grid);
}
