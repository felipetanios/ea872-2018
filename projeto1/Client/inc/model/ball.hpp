#ifndef BALL_GAMEOBJECT
#define BALL_GAMEOBJECT

#include <model/gameobject.hpp>

class Ball : public GameObject {
public:
	double xSpeed;
	double ySpeed;
    Ball(float radius, float x, float y);
	bool collided = false;
    void update();
};
#endif