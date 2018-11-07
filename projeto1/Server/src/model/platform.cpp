#include <model/platform.hpp>

#define MOVE_INCREMENT 0.3f


Platform::Platform() {
	x = 0.f;
	y = -3.f;
	z = -8.f;
	width = 2.f;
	height = .3f;
	depth = 1.f;
	r = 1.f;
	g = 0.f;
	b = 0.f;
    rendererType = RendererType_Cube;

	this->updateCollisionLogic();
}

void Platform::moveRight() {
	x += MOVE_INCREMENT;
	this->updateCollisionLogic();
}

void Platform::moveLeft() {
	x -= MOVE_INCREMENT;
	this->updateCollisionLogic();
}