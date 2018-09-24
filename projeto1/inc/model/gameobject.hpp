#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <view/renderer.hpp>
#include <model/line.hpp>
#include <list>

using namespace std;

class GameObject {
public:
    static int getTotalObjects();
    int getId();

	Renderer *renderer; // view logic
    float x, y, z; // position
    float r, g, b; // color
    float width, height, depth; // size
    virtual void update();

    // collision logic
    float left, right, top, bottom;
    list<Line*> lines;
    virtual void updateCollisionLogic();

protected:
    int id;
    static int totalObjects;
    int setId();
};

#endif