#include <model/platform.hpp>
#include <view/cuberenderer.hpp>

#define MOVE_INCREMENT 0.1f


Platform::Platform() {
	x = 0.f;
	y = -3.f;
	z = -8.f;
	width = 4.f;
	height = .3f;
	depth = 1.f;
	r = 1.f;
	g = 0.f;
	b = 0.f;

	renderer = new CubeRenderer();
	renderer->setSize(width, height, depth);
	renderer->setColor(r, g, b);
	renderer->setPosition(x, y, z);
}

Platform::~Platform() {
	delete renderer;
}

void Platform::moveRight() {
	x += MOVE_INCREMENT;
	renderer->setPosition(x, y, z);
}

void Platform::moveLeft() {
	x -= MOVE_INCREMENT;
	renderer->setPosition(x, y, z);
}