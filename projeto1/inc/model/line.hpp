#ifndef LINE_TYPE
#define LINE_TYPE

#include <cmath>
using namespace std;

class Line {
public:
	bool isVertical;
	union {
		struct {
			float x, y1, y2;
		} vertical;
		struct {
			float y, x1, x2;
		} horizontal;
	};
	bool intersects(Line other);
	static Line* getVertical(float x, float y1, float y2);
	static Line* getHorizontal(float y, float x1, float x2);
	~Line();

private:
	Line();
};
#endif