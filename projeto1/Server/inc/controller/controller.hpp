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

	static mutex receiverThreadsMtx;
	static map<int, thread> receiverThreads;
	static map<int, GameObject*> gameObjects;
	static map<int, Ball*> balls;

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
	
	static void keyboardHandler(int platformId, int socket_fd, int connection_fd, struct sockaddr_in client);

	static thread messageSenderThread;
	static void messageSender();

	static thread connectionHandlerThread;
	static void connectionHandler(int socket_fd, struct sockaddr_in client);

	static queue<NetworkMessage> pendingMessages;
	static mutex msgQueueMtx;

	static list<NetworkMessage> permanentMessages;
	static mutex permanentMsgQueueMtx;

	static void init();
	static void update();
	static void enqueueMessage(NetworkMessage msg);
	static void sendMessage(NetworkMessage msg, int socket_fd);

    static void applyDeletions();
    static void markForDeletion(int id);
    static list<int> toBeDeleted;
};

