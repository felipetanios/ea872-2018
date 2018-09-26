#ifndef BALL_GAMEOBJECT
#define BALL_GAMEOBJECT

#include <model/gameobject.hpp>

class Ball : public GameObject {
public:
	double xSpeed;
	double ySpeed;
	bool collided = false;
    Ball(float radius);
    ~Ball();
    void update();
};
#endif