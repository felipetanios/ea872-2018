#include <list>
#include <queue>
#include <map>
#include <view/audio.hpp>
#include <view/renderer.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include "../../inc/datatypes.hpp"

using namespace std;
using namespace Audio;

class Controller {
public:
	static list<thread> soundThreads;
	static map<int, Renderer> renderers;
	static mutex mtx;
	static queue<char> keyboardBuffer;
	static Sample *asample;
	static Player *player;
	static thread senderThread, receiverThread;
	static int connectionId;
	static int socketId;
	
	static bool ended;
	static int score;

	static void init();
	static void readKeyboardInput(unsigned char key, int x, int y);
	static void receiverLoop();
	static void senderLoop();
 	static void threadSound (Player *player, Sample *asample);

};
