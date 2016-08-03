#ifndef JNB_SFML_TEXTURE_MANAGER_H
#define JNB_SFML_TEXTURE_MANAGER_H

#include <unordered_map>
#include <SFML/Graphics/Texture.hpp>

struct TextureManager{
    std::unordered_map<const char*, sf::Texture*> texture_map;

    sf::Texture& get_texture(const char* name);
    static TextureManager& inst();

private:
    TextureManager();

};


#endif //JNB_SFML_TEXTURE_MANAGER_H
