#include <model/ball.hpp>
#include <view/sphererenderer.hpp>
#include <controller/controller.hpp>
#include <list>
#include <cmath>

using namespace std;

#define COLLISION_THRESHOLD 0.1f

Ball::Ball(float radius) {
	this->setId();
	x = 2.f;
	y = 0.f;
	z = -8.f;
	width = radius;
	height = radius;
	depth = radius;
	r = 0.f;
	g = 1.f;
	b = 0.f;
	xSpeed = 0.05f;
	ySpeed = 0.05f;

	renderer = new SphereRenderer();
	renderer->setSize(width, height, depth);
	renderer->setColor(r, g, b);
	renderer->setPosition(x, y, z);
}

Ball::~Ball() {
	delete renderer;
}

void Ball::update() {
	// gather world collidable lines
	list<VerticalLine> worldVLines = {};
	list<HorizontalLine> worldHLines = {};
	list<GameObject*>::iterator it;
    for (it = Controller::gameObjects.begin(); it != Controller::gameObjects.end(); ++it) {
    	if ((*it)->getId() != this->getId()) {
	        worldVLines.insert(worldVLines.end(), (*it)->vLines.begin(), (*it)->vLines.end());
	        worldHLines.insert(worldHLines.end(), (*it)->hLines.begin(), (*it)->hLines.end());
	    }
    }

    // detect collision between vertical lines
    bool vCollision = false;
	list<VerticalLine>::iterator it1, it2;
    for (it1 = worldVLines.begin(); it1 != worldVLines.end() && !vCollision; ++it1) {
    	for (it2 = vLines.begin(); it2 != vLines.end() && !vCollision; ++it2) {
    		if ((*it1).intersects(*it2)) {
    			vCollision = true;
    		}
    	}
    }
    if (vCollision) xSpeed *= -1;

    // detect collision between horizontal lines
    bool hCollision = false;
	list<HorizontalLine>::iterator it3, it4;
    for (it3 = worldHLines.begin(); it3 != worldHLines.end() && !hCollision; ++it3) {
    	for (it4 = hLines.begin(); it4 != hLines.end() && !hCollision; ++it4) {
    		if ((*it3).intersects(*it4)) {
    			hCollision = true;
    		}
    	}
    }
    if (hCollision) ySpeed *= -1;


	x += xSpeed;
	y += ySpeed;
	/*
	*@TODO: Detect collision
	*/
	renderer->setPosition(x, y, z);
	this->updateCollisionLogic();
}