#include <list>
#include <model/platform.hpp>
#include <model/ball.hpp>
#include <view/audio.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace Audio;

class Controller {
public:
	static list<thread> soundThreads;
	static Platform *platform;
	static Ball *ball;
	static Sample *asample;
	static Player *player;
	static thread keyboardThread;
	static map<int, thread> connection;

	static void init();
	static void update();
	static void readKeyboardInput(unsigned char key, int x, int y);
	static void connectionHandler(int connectionID);

};

void threadSound (Player *player, Sample *asample);
void threadServerKeyboard(Platform *platform);
