#include <model/ball.hpp>
#include <model/brick.hpp>
#include <controller/controller.hpp>
#include <list>
#include <cmath>
#include <iostream>

using namespace std;

#define COLLISION_THRESHOLD 0.1f

Ball::Ball(float radius, float x, float y) {
	//creates the ball object with it renderer, position, color, size and speed
	this->x = x;
	this->y = y;
	z = -8.f;
	width = radius;
	height = radius;
	depth = radius;
	r = 1.f;
	g = 0.f;
	b = 0.f;
	xSpeed = 0.05f;
	ySpeed = 0.05f;
	collided = false;
    rendererType = RendererType_Sphere;
}

void Ball::update() {
	//updates ball position
	//to do that we check if it has colided with any wall of the boundaries walls first

	//to do that we gather all the boundaries lines
	list<shared_ptr<Line>> worldLines = {};
    map<int, GameObject*>::iterator it;
    for (it = Controller::gameObjects.begin(); it != Controller::gameObjects.end(); ++it) {
    	if (!it->second->deleted && it->second->getId() != this->getId() && (it->second->owner == 0 || it->second->owner == this->owner)) {
	        lines.insert(worldLines.end(), it->second->lines.begin(), it->second->lines.end());
	    }
    }

    // after that we check if the ball has colided (intersected) with any walls within the boundaries walls list
    //if it has collided, we can check if it is a vertial or horizontal collision
    //it is important to do that so that we can change y and x speed direction at the end of the method
    int collidedElement = -1;
    bool vCollision = false;
    bool hCollision = false;
    list<shared_ptr<Line>>::iterator ballLine, worldLine;
    for (worldLine = worldLines.begin(); worldLine != worldLines.end(); ++worldLine) {
    	for (ballLine = lines.begin(); ballLine != lines.end(); ++ballLine) {
    		if ((*ballLine)->intersects(**worldLine)) {
    			collidedElement = (*worldLine)->ownerId;
    			if ((*ballLine)->isVertical) {
    				vCollision = true;
    			} else {
    				hCollision = true;
    			}
    			break;
    		}
    	}
    }
    collided = false;
    //afterwards, we set the collided flag to true (this is the flag that we use to generate the sound thread)

    if (vCollision == true || hCollision == true ){
        collided = true;
    }
	if (collidedElement != -1 && Controller::gameObjects.find(collidedElement) != Controller::gameObjects.end()) {
        GameObject *collidedGameObject = Controller::gameObjects[collidedElement];
        if (collidedGameObject->destroyable) {
            Controller::markForDeletion(collidedGameObject->getId());
            Controller::brickCounter--;
            Controller::scores[this->owner]++;
        }
		collided = true;
	}

    //the last step is (as said before) to change x and y speed because of the colisions
    if (vCollision) xSpeed *= -1;
    if (hCollision) ySpeed *= -1;
	x += xSpeed;
	y += ySpeed;

	// check if out of boundaries
    if (x > 10 || x < -10 || y > 10 || y < -10) {
        Controller::markForDeletion(this->getId());
    } else {
    	//and finally we render the ball again
    	this->updateCollisionLogic();
        Controller::sendNewPosition(*this);
        if (collided) Controller::sendSound();
    }
}