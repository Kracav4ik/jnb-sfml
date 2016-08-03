#ifndef JNB_SFML_UTILS_H
#define JNB_SFML_UTILS_H

#include <SFML/Graphics.hpp>

#define FOREACH(LIST_TYPE, ITEM_TYPE, LIST, code)                                   \
for (LIST_TYPE<ITEM_TYPE>::iterator it = LIST.begin(); it != LIST.end(); ++it) {    \
    ITEM_TYPE value = *it;                                                          \
    code                                                                            \
}

#define FOREACH_C(LIST_TYPE, ITEM_TYPE, LIST, code)                                     \
for (LIST_TYPE<ITEM_TYPE>::const_iterator it = LIST.begin(); it != LIST.end(); ++it) {  \
    ITEM_TYPE value = *it;                                                              \
    code                                                                                \
}

using namespace sf;

void draw_rect(RenderWindow& window, Vector2f pos, Vector2f size, Color color, bool fill=true);

void draw_line(RenderWindow& window, Vector2f pos1, Vector2f pos2, Color color);

#endif //JNB_SFML_UTILS_H
