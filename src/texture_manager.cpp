
#include "texture_manager.h"


sf::Texture& TextureManager::get_texture(const char* name) {
    if (texture_map.count(name)){
        return *texture_map[name];
    }
    sf::Texture* texture = new sf::Texture;
    (*texture).loadFromFile(name);
    texture_map[name] = texture;
    return *texture;
}

TextureManager& TextureManager::inst() {
    static TextureManager manager;
    return manager;
}

TextureManager::TextureManager() {

}
