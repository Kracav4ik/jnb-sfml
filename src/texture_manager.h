#ifndef JNB_SFML_TEXTURE_MANAGER_H
#define JNB_SFML_TEXTURE_MANAGER_H

#include <unordered_map>
#include <SFML/Graphics.hpp>
using namespace sf;


struct TextureManager{
    std::unordered_map<std::string, Texture*> texture_map;
    Texture empty;

    Texture& get_texture(const std::string& name);
    static TextureManager& inst();

private:
    TextureManager();

};


#endif //JNB_SFML_TEXTURE_MANAGER_H
