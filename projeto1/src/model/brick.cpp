#include <model/brick.hpp>

Brick::Brick(float x, float y) : Box(x, y, -8.f, .5f, .25f) { }

void Brick::collide() {
	//this method is called when we have to delete the brick (when the ball has collided with the brick)
	this->markForDeletion();
}