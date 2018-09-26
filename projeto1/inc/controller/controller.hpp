#include <list>
#include <model/platform.hpp>
#include <model/ball.hpp>
#include <view/audio.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;
using namespace Audio;

class Controller {
public:
	static thread soundThread;
	static list<GameObject*> gameObjects;
	static list<Renderer*> renderers;
	static Platform *platform;
	static Ball *ball;
	static Sample *asample;
	static Player *player;


	static void init();
	static void update();
	static void readKeyboardInput(unsigned char key, int x, int y);

};

void threadSound (Player *player, Sample *asample);