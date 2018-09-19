#include <model/gameobject.hpp>

class Ball : public GameObject {
public:
	double xSpeed;
	double ySpeed;
    Ball();
    ~Ball();
    void update();
};
