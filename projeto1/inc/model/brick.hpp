#include <model/box.hpp>

class Brick : public Box {
public:
	void collide(GameObject *other);
	Brick(float x, float y);
};