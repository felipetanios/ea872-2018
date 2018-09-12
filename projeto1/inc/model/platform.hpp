#include <model/gameobject.hpp>

class Platform : public GameObject {
public:
    Platform();
    ~Platform();
    void moveRight();
    void moveLeft();
};
