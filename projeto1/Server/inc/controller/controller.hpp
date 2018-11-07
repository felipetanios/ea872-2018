#include <list>
#include <model/platform.hpp>
#include <model/ball.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>
#include <mutex>
#include "../../../inc/datatypes.hpp"

using namespace std;

class Controller {
public:
	static list<thread> soundThreads;
	static Platform *platform;
	static Ball *ball;
	static map<int, thread> connection;

	static void sendNewObject(GameObject newGameObject);
	static void sendNewPosition(GameObject gameObject);
	static void sendDestroy(GameObject GameObject);
	static void sendSound();

	static int sequence;
	
	static thread keyboardHandlerThread;
	static void keyboardHandler(Platform *platform, int socket_fd, int connection_fd, struct sockaddr_in client);

	static thread messageSenderThread;
	static void messageSender(int socket_fd);

	static queue<NetworkMessage> pendingMessages;
	static mutex msgQueueMtx;

	static void init();
	static void update();
	static void sendMessage(NetworkMessage msg);
};

