#include <list>
#include <queue>
#include <view/audio.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace Audio;

class Controller {
public:
	static list<thread> soundThreads;
	static mutex mtx;
	static queue<char> keyboardBuffer;
	static Sample *asample;
	static Player *player;
	static thread networkThread;
	

	static void init();
	static void readKeyboardInput(unsigned char key, int x, int y);

};

void threadSound (Player *player, Sample *asample);
void networkHandler();