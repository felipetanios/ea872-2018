#ifndef PLATFORM
#define PLATFORM

#include <model/gameobject.hpp>

class Platform : public GameObject {
public:
    Platform();
    void moveRight();
    void moveLeft();
};

#endif