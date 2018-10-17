#ifndef LINE_TYPE
#define LINE_TYPE

#include <cmath>
#include <memory>
using namespace std;

class Line {
public:
	bool isVertical;
	int ownerId;
	union {
		struct {
			float x, y1, y2;
		} vertical;
		struct {
			float y, x1, x2;
		} horizontal;
	};
	bool intersects(Line other);
	static shared_ptr<Line> getVertical(float x, float y1, float y2, int ownerId);
	static shared_ptr<Line> getHorizontal(float y, float x1, float x2, int ownerId);

private:
	Line();
};
#endif