#ifndef SIMPLESCENEGRAPH_FPS_H_
#define SIMPLESCENEGRAPH_FPS_H_


#include <deque>

class FpsCounter {
private:
    std::deque<Uint32> frames;
public:
    float addFrame(Uint32 t) {
        frames.push_back(t);
        while (frames.size() > 64) {
            frames.pop_front();
        }
        return static_cast<float>(frames.back() - frames.front()) / static_cast<float>(frames.size());
    }
};

#endif
