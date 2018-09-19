#include <model/ball.hpp>
#include <view/sphererenderer.hpp>

Ball::Ball() {
	x = 0.f;
	y = 0.f;
	z = -8.f;
	// width = 10.f;
	// height = 1.f;
	// depth = 3.f;
	r = 0.f;
	g = 1.f;
	b = 0.f;
	xSpeed = 0.2f;
	ySpeed = -0.2f;

	renderer = new SphereRenderer();
	//renderer->setSize(width, height, depth);
	renderer->setColor(r, g, b);
	renderer->setPosition(x, y, z);
}

Ball::~Ball() {
	delete renderer;
}

void Ball::update() {
	x += 0.1f;
	renderer->setPosition(x, y, z);
}
