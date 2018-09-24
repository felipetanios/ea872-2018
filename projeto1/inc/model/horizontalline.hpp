#ifndef HORIZONTAL_LINE
#define HORIZONTAL_LINE

#include <cmath>
#include <model/verticalline.hpp>
using namespace std;

#define COLLISION_THRESHOLD 0.1f

class VerticalLine;
class HorizontalLine {
	float y;
	float x1;
	float x2;

	bool intersects(HorizontalLine other) {
		bool isSameY = abs(other.y - this->y) < COLLISION_THRESHOLD;
		if (isSameY) {
			// check if other.x1 is between this x1 and x2
 			if (this->x1 > other.x1 && this->x2 < other.x1)
 				return true;
 			if (this->x1 < other.x1 && this->x2 > other.x1)
 				return true;

 			// check if other y2 is between this y1 and y2
 			if (this->x1 > other.x2 && this->x2 < other.x2)
 				return true;
 			if (this->x1 < other.x2 && this->x2 > other.x2)
 				return true;

 			// check if this y1 is between other y1 and y2
 			if (other.x1 > this->x1 && other.x2 < this->x1)
 				return true;
 			if (other.x1 < this->x1 && other.x2 > this->x1)
 				return true;

 			// check if this y2 is between other y1 and y2
 			if (other.x1 > this->x2 && other.x2 < this->x2)
 				return true;
 			if (other.x1 < this->x2 && other.x2 > this->x2)
 				return true;
		}
		return false;
	};

	bool intersects(VerticalLine other) {
		return other.intersects(this);
	}
};
#endif