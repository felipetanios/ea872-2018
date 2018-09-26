#include <model/line.hpp>

#define COLLISION_THRESHOLD 0.01f

shared_ptr<Line> Line::getVertical(float x, float y1, float y2, int ownerId) {
	Line* newLine = new Line();
	newLine->isVertical = true;
	newLine->vertical.x = x;
	newLine->vertical.y1 = y1;
	newLine->vertical.y2 = y2;
	newLine->ownerId = ownerId;
	return shared_ptr<Line>(newLine);
}

shared_ptr<Line> Line::getHorizontal(float y, float x1, float x2, int ownerId) {
	Line* newLine = new Line();
	newLine->isVertical = false;
	newLine->horizontal.y = y;
	newLine->horizontal.x1 = x1;
	newLine->horizontal.x2 = x2;
	newLine->ownerId = ownerId;
	return shared_ptr<Line>(newLine);
}

bool Line::intersects(Line other) {
	// first case: both are vertical
	if (this->isVertical && other.isVertical) {
		bool isSameX = abs(other.vertical.x - this->vertical.x) < COLLISION_THRESHOLD;
		if (isSameX) {
			// check if other.y1 is between this y1 and y2
 			if (this->vertical.y1 > other.vertical.y1 && this->vertical.y2 < other.vertical.y1)
 				return true;
 			if (this->vertical.y1 < other.vertical.y1 && this->vertical.y2 > other.vertical.y1)
 				return true;

 			// check if other y2 is between this y1 and y2
 			if (this->vertical.y1 > other.vertical.y2 && this->vertical.y2 < other.vertical.y2)
 				return true;
 			if (this->vertical.y1 < other.vertical.y2 && this->vertical.y2 > other.vertical.y2)
 				return true;

 			// check if this y1 is between other y1 and y2
 			if (other.vertical.y1 > this->vertical.y1 && other.vertical.y2 < this->vertical.y1)
 				return true;
 			if (other.vertical.y1 < this->vertical.y1 && other.vertical.y2 > this->vertical.y1)
 				return true;

 			// check if this y2 is between other y1 and y2
 			if (other.vertical.y1 > this->vertical.y2 && other.vertical.y2 < this->vertical.y2)
 				return true;
 			if (other.vertical.y1 < this->vertical.y2 && other.vertical.y2 > this->vertical.y2)
 				return true;
		}
		return false;
	}

	// second case: both are horizontal
	else if (!this->isVertical && !other.isVertical) {
		bool isSameY = abs(other.horizontal.y - this->horizontal.y) < COLLISION_THRESHOLD;
		if (isSameY) {
			// check if other.x1 is between this x1 and x2
 			if (this->horizontal.x1 > other.horizontal.x1 && this->horizontal.x2 < other.horizontal.x1)
 				return true;
 			if (this->horizontal.x1 < other.horizontal.x1 && this->horizontal.x2 > other.horizontal.x1)
 				return true;

 			// check if other y2 is between this y1 and y2
 			if (this->horizontal.x1 > other.horizontal.x2 && this->horizontal.x2 < other.horizontal.x2)
 				return true;
 			if (this->horizontal.x1 < other.horizontal.x2 && this->horizontal.x2 > other.horizontal.x2)
 				return true;

 			// check if this y1 is between other y1 and y2
 			if (other.horizontal.x1 > this->horizontal.x1 && other.horizontal.x2 < this->horizontal.x1)
 				return true;
 			if (other.horizontal.x1 < this->horizontal.x1 && other.horizontal.x2 > this->horizontal.x1)
 				return true;

 			// check if this y2 is between other y1 and y2
 			if (other.horizontal.x1 > this->horizontal.x2 && other.horizontal.x2 < this->horizontal.x2)
 				return true;
 			if (other.horizontal.x1 < this->horizontal.x2 && other.horizontal.x2 > this->horizontal.x2)
 				return true;
		}
		return false;
	}

	// third case: one is horizontal and the other is vertical
	Line v = other.isVertical ? other : (*this);
	Line h = other.isVertical ? (*this) : other;

	bool intersectsX = (v.vertical.x > h.horizontal.x1 && v.vertical.x < h.horizontal.x2)
		|| (v.vertical.x < h.horizontal.x1 && v.vertical.x > h.horizontal.x2);
	
	bool intersectsY = (h.horizontal.y > v.vertical.y1 && h.horizontal.y < v.vertical.y2)
		|| (h.horizontal.y < v.vertical.y1 && h.horizontal.y > v.vertical.y2);
	
	return intersectsX && intersectsY;
}

Line::Line() {}