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
	list<Line*> worldLines = {};
	list<GameObject*>::iterator it;
    for (it = Controller::gameObjects.begin(); it != Controller::gameObjects.end(); ++it) {
    	if ((*it)->getId() != this->getId()) {
	        lines.insert(worldLines.end(), (*it)->lines.begin(), (*it)->lines.end());
	    }
    }

    bool vCollision = false;
    bool hCollision = false;
    list<Line*>::iterator ballLine, worldLine;
    for (worldLine = worldLines.begin(); worldLine != worldLines.end(); ++worldLine) {
    	for (ballLine = lines.begin(); ballLine != lines.end(); ++ballLine) {
    		if ((*ballLine)->intersects(**worldLine)) {
    			if ((*ballLine)->isVertical) {
    				vCollision = true;
    				break;
    			} else {
    				hCollision = true;
    				break;
    			}
    		}
    	}
    }

    if (vCollision) xSpeed *= -1;
    if (hCollision) ySpeed *= -1;
	x += xSpeed;
	y += ySpeed;
	
	renderer->setPosition(x, y, z);
	this->updateCollisionLogic();
}