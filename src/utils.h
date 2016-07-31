#ifndef JNB_SFML_UTILS_H
#define JNB_SFML_UTILS_H

#include <SFML/Graphics.hpp>

using namespace sf;

void draw_rect(RenderWindow& window, Vector2f pos, Vector2f size, Color color, bool fill=true);

void draw_line(RenderWindow& window, Vector2f pos1, Vector2f pos2, Color color);

#endif //JNB_SFML_UTILS_H
