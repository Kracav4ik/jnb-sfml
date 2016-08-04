#ifndef JNB_SFML_ANIMATION_H
#define JNB_SFML_ANIMATION_H

struct Animation{
    float time;
    float total_time;

    Animation();

    void step(float elapsed);


};

#endif //JNB_SFML_ANIMATION_H
