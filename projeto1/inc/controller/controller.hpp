#include <list>
#include <model/platform.hpp>
#include <model/ball.hpp>

using namespace std;

class Controller {
public:
	static list<GameObject> gameObjects;
	static list<Renderer*> renderers;
	static Platform platform;
	static Ball ball;


	static void init();
	static void update();
	static void readInput();
};