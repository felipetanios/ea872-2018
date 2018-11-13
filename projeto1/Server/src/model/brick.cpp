#include <model/brick.hpp>
#include <controller/controller.hpp>

Brick::Brick(float x, float y) : Box(x, y, -8.f, .5f, .25f) {
	this->destroyable = true;
}
