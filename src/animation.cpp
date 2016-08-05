
#include "animation.h"

Animation::Animation()
    :time(0), total_time(1)
{
}

void Animation::step(float elapsed) {
    time += elapsed;
    if (time >= total_time) {
        time -= int(time / total_time) * total_time;
    }
}
