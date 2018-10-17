#ifndef GAME_OBJECT
#define GAME_OBJECT

#include <view/renderer.hpp>
#include <model/line.hpp>
#include <list>
#include <map>
#include <memory>

using namespace std;

class GameObject {
public:
    static int getTotalObjects();
    static map<int, shared_ptr<GameObject>> gameObjects;
    static void applyDeletions();

    bool deleted = false;
    int getId();

	Renderer renderer; // view logic
    float x, y, z; // position
    float r, g, b; // color
    float width, height, depth; // size
    virtual void update();
    
    // collision logic
    float left, right, top, bottom;
    list<shared_ptr<Line>> lines;
    virtual void updateCollisionLogic();
    virtual void collide();

    GameObject();
    ~GameObject();

protected:
    static list<int> toBeDeleted;
    void markForDeletion();
    int id;
    static int totalObjects;
    int setId();
};

#endif