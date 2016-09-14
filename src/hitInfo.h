#ifndef JNB_SFML_HITINFO_H
#define JNB_SFML_HITINFO_H


struct HitInfo {
    HitInfo(){
        reset();
    }

    bool hit_x;
    bool hit_y;

    void reset() {
        hit_x = false;
        hit_y = false;
    }
};



#endif //JNB_SFML_HITINFO_H
