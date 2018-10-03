#include <model/ball.hpp>
#include <view/sphererenderer.hpp>
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
	r = 0.f;
	g = 1.f;
	b = 0.f;
	xSpeed = 0.05f;
	ySpeed = 0.05f;
	collided = false;

	renderer = SphereRenderer();
	renderer.setSize(width, height, depth);
	renderer.setColor(r, g, b);
	renderer.setPosition(x, y, z);
}

void Ball::update() {
	//updates ball position
	//to do that we check if it has colided with any wall of the boundaries walls first

	//to do that we gather all the boundaries lines
	list<shared_ptr<Line>> worldLines = {};
    map<int, shared_ptr<GameObject>>::iterator it;
    for (it = GameObject::gameObjects.begin(); it != GameObject::gameObjects.end(); ++it) {
    	if (!it->second->deleted && it->second->getId() != this->getId()) {
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

    //afterwards, we set the collided flag to true (this is the flag that we use to generate the sound thread)
	if (collidedElement != -1 && GameObject::gameObjects.find(collidedElement) != GameObject::gameObjects.end()) {
		GameObject::gameObjects[collidedElement]->collide();
		collided = true;
	}
    if (vCollision == true || hCollision == true ){
    	collided = true;
    }

    //the last step is (as said before) to change x and y speed because of the colisions
    if (vCollision) xSpeed *= -1;
    if (hCollision) ySpeed *= -1;
	x += xSpeed;
	y += ySpeed;
	
	//and finally we render the ball again
	renderer.setPosition(x, y, z);
	this->updateCollisionLogic();

}