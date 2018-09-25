#include <model/ball.hpp>
#include <view/sphererenderer.hpp>
#include <controller/controller.hpp>
#include <list>
#include <cmath>
#include <iostream>

using namespace std;

#define COLLISION_THRESHOLD 0.1f

Ball::Ball(float radius, float x, float y) {
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

	renderer = SphereRenderer();
	renderer.setSize(width, height, depth);
	renderer.setColor(r, g, b);
	renderer.setPosition(x, y, z);
}

void Ball::update() {
	list<Line*> worldLines = {};
    map<int, GameObject*>::iterator it;
    for (it = GameObject::gameObjects.begin(); it != GameObject::gameObjects.end(); ++it) {
    	if (!it->second->deleted && it->second->getId() != this->getId()) {
	        lines.insert(worldLines.end(), it->second->lines.begin(), it->second->lines.end());
	    }
    }

    int collided = -1;
    bool vCollision = false;
    bool hCollision = false;
    list<Line*>::iterator ballLine, worldLine;
    for (worldLine = worldLines.begin(); worldLine != worldLines.end(); ++worldLine) {
    	for (ballLine = lines.begin(); ballLine != lines.end(); ++ballLine) {
    		if ((*ballLine)->intersects(**worldLine)) {
    			collided = (*worldLine)->ownerId;
    			if ((*ballLine)->isVertical) {
    				vCollision = true;
    			} else {
    				hCollision = true;
    			}
    			break;
    		}
    	}
    }

	if (collided != -1 && GameObject::gameObjects.find(collided) != GameObject::gameObjects.end()) {
		GameObject::gameObjects[collided]->collide();
	}

    if (vCollision) xSpeed *= -1;
    if (hCollision) ySpeed *= -1;
	x += xSpeed;
	y += ySpeed;
	
	renderer.setPosition(x, y, z);
	this->updateCollisionLogic();

}