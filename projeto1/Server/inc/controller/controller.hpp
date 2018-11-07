#include <list>
#include <model/platform.hpp>
#include <model/ball.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include "../../../inc/datatypes.hpp"

using namespace std;

class Controller {
public:
	static list<thread> soundThreads;
	static Platform *platform;
	static Ball *ball;
	static map<int, thread> connection;
	
	static thread keyboardHandlerThread;
	static void keyboardHandler(Platform *platform, int socket_fd);

	static thread messageSenderThread;
	static void messageSender(int socket_fd);

	static queue<NetworkMessage> pendingMessages;
	static mutex msgQueueMtx;

	static void init();
	static void update();
	static void sendMessage(NetworkMessage msg);
};

