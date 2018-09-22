
#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <view/renderer.hpp>

class GameObject {
public:
	Renderer *renderer;
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float width;
    float height;
    float depth;
    virtual void update() {}; //oqissofazsocorro;
};
#endif