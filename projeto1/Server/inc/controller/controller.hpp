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
	static map<int, thread> connection;
	static map<int, GameObject> gameObjects;


	static int createBall(float radius, float x, float y);
	static int createBox(float x, float y, float z, float width, float height);
	static int createBrick(float x, float y);
	static int createPlatform();

	static void sendNewObject(GameObject newGameObject);
	static void sendNewPosition(GameObject gameObject);
	static void sendNewPosition(int gameObjectId, float x, float y, float z);
	static void sendDestroy(int id);
	static void sendSound();

	static int sequence;
	
	static thread keyboardHandlerThread;
	static void keyboardHandler(int platformId, int socket_fd, int connection_fd, struct sockaddr_in client);

	static thread messageSenderThread;
	static void messageSender(int socket_fd);

	static queue<NetworkMessage> pendingMessages;
	static mutex msgQueueMtx;

	static void init();
	static void update();
	static void sendMessage(NetworkMessage msg);

    static void applyDeletions();
    static void markForDeletion(int id);
    static list<int> toBeDeleted;
};

