#include <model/box.hpp>
#include <view/cuberenderer.hpp>

Box::Box(float x, float y, float z, float width, float height) {	
	this->x = x;
	this->y = y;
	this->z = z;
	this->width = width;
	this->height = height;
	depth = 1.f;	
	r = 0.f;
	g = 0.f;
	b = 1.f;

	this->updateCollisionLogic();

	renderer = new CubeRenderer();
	renderer->setSize(this->width, this->height, depth);
	renderer->setColor(r, g, b);
	renderer->setPosition(this->x, this->y, this->z);

}
