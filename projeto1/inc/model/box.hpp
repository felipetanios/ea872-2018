#include <model/gameobject.hpp>

class Box : public GameObject {
public:
    Box(float x, float y, float z, float width, float height);
    ~Box();
};
