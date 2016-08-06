
#include <string.h>
#include "texture_manager.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
struct PCXPalette {
    struct PCXColor {
        Uint8 r, g, b;
    };

    Uint8 signature;
    PCXColor colors[256];
};

struct PCXHeader {
    Uint8 manufacturer;
    Uint8 version;
    Uint8 encoding;
    Uint8 bitsPerPlane;
    Uint16 Xmin;
    Uint16 Ymin;
    Uint16 Xmax;
    Uint16 Ymax;
    Uint16 vertDPI;
    Uint16 horzDPI;
    Uint8 palette[48];
    Uint8 reserved;
    Uint8 colorPlanes;
    Uint16 bytesPerPlaneLine;
    Uint16 paltype;
    Uint16 hScrSize;
    Uint16 vScrSize;
    Uint8 pad[54];
};
#pragma clang diagnostic pop

bool load_from_pcx(Texture& texture, const std::string& name) {
    FILE* file = fopen(name.c_str(), "rb");
    PCXHeader header;
    fread(&header, sizeof(header), 1, file);

    unsigned int width = header.Xmax + 1;
    unsigned int height = header.Ymax + 1;
    int array_size = width * height;
    Uint8* image_data = new Uint8[array_size];

    int i = 0;

    while (i < array_size) {
        Uint8 u;
        fread(&u, 1, 1, file);
        if (u < 0xC0) {
            image_data[i] = u;
            i++;
        } else {
            Uint8 uu;
            fread(&uu, 1, 1, file);
            u &= 0x3f;
            for (int _ = 0; _ < u; _++) {
                image_data[i] = uu;
                i++;
            }
        }
    }

    PCXPalette palette;
    fread(&palette, sizeof(palette), 1, file);

    Image image;
    image.create(width, height);

    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            int offset = y*width + x;
            const PCXPalette::PCXColor& color = palette.colors[image_data[offset]];
            image.setPixel(x, y, Color(color.r, color.g, color.b));
        }
    }
//    image.saveToFile("c:\\git_guest\\test.png");
    return texture.loadFromImage(image);
}

Texture& TextureManager::get_texture(const std::string& name) {
    if (texture_map.count(name)) {
        return *texture_map[name];
    }
    Texture* texture = new Texture();
    bool loaded;
    if (name.substr(name.size() - 4) == ".pcx") {
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
