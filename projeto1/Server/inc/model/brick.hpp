#include <model/box.hpp>

class Brick : public Box {
public:
	void collide();
	Brick(float x, float y);
};