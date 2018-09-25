#include <list>
#include <model/platform.hpp>
#include <model/ball.hpp>

using namespace std;

class Controller {
public:
	static Platform *platform;
	static Ball *ball;
	static void init();
	static void update();
	static void readKeyboardInput(unsigned char key, int x, int y);
};