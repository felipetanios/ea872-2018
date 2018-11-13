#include <controller/controller.hpp>
#include <model/brick.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstdio>

#include <chrono>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>
#include <cerrno>
#include <cstring>

#define ever ;; 

using namespace std;

uint64_t get_now_ms() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

thread Controller::connectionHandlerThread;

queue<NetworkMessage> Controller::pendingMessages;
mutex Controller::msgQueueMtx;

list<NetworkMessage> Controller::permanentMessages;
mutex Controller::permanentMsgQueueMtx;
mutex Controller::receiverThreadsMtx;

//map<int, thread> Controller::senderThreads;
map<int, thread> Controller::receiverThreads;
map<int, GameObject*> Controller::gameObjects;
map<int, Ball*> Controller::balls;
list<int> Controller::toBeDeleted;

thread Controller::messageSenderThread;

int Controller::sequence;

void Controller::init() {
    signal(SIGPIPE, SIG_IGN); 
    int socket_fd, connection_fd;
    struct sockaddr_in myself, client;
    
    char input_buffer[50];

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket criado\n");

    myself.sin_family = AF_INET;
    myself.sin_port = htons(3001);
    inet_aton("127.0.0.1", &(myself.sin_addr));

    printf("Tentando abrir porta 3001\n");

    if (::bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) < 0) {
        printf("Problemas ao abrir porta\n");
    }
    printf("Abri porta 3001!\n");

    listen(socket_fd, 2);
    printf("Estou ouvindo na porta 3001!\n");

    Controller::connectionHandlerThread = thread(Controller::connectionHandler, socket_fd, client);
    Controller::messageSenderThread = thread(Controller::messageSender);

    printf("Conectado");
    

    srand(time(NULL));

	Controller::createBox(0.f, 3.5f, -8.f, 10.5f, .5f);
	Controller::createBox(-5.f, -1.5f, -8.f, .5f, 10.f);
	Controller::createBox(5.f, -1.5f, -8.f, .5f, 10.f);

    for (int i=-7; i<=7; i++) {
        for (int j=0; j<5; j++) {
            Controller::createBrick(i * .6f, 2.5f - j*.5f);
        }
    }

    for(ever) Controller::update();
}


void Controller::update() {
    //update ball positions
    map<int, Ball*>::iterator it1;
    for (it1 = Controller::balls.begin(); it1 != Controller::balls.end(); ++it1) {
        it1->second->update();
    }

    Controller::applyDeletions();

    //detects collision and make sound
    // if (Controller::ball->collided == true){
    //     //when a collision is detected create a new thread that rouns a method to play the sound effect
    //     printf("Colided\n");
    //     Controller::ball->collided = false;
    // }
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
}

void Controller::enqueueMessage(NetworkMessage msg) {
    Controller::msgQueueMtx.lock();
    msg.sequence = ++Controller::sequence;
    Controller::pendingMessages.push(msg);
    Controller::msgQueueMtx.unlock();
}

void Controller::connectionHandler(int socket_fd, struct sockaddr_in client){
    socklen_t client_size = (socklen_t)sizeof(client);
    for(ever){
        int connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
        if (connection_fd < 0) {
            cerr << "ERROR: " << strerror(errno) << endl;
        } else {
            cout << "Starting new thread for new connection handling" << endl;

            Controller::permanentMsgQueueMtx.lock();
            list<NetworkMessage>::iterator it1;
            for (it1 = Controller::permanentMessages.begin(); it1 != Controller::permanentMessages.end(); ++it1) {
                NetworkMessage msg = *it1;
                Controller::sendMessage(msg, connection_fd);
            }
            Controller::permanentMsgQueueMtx.unlock();

            Controller::receiverThreadsMtx.lock();
            Controller::receiverThreads[connection_fd] = thread(Controller::keyboardHandler, Controller::createPlatform(), socket_fd, connection_fd, client);
            Controller::receiverThreadsMtx.unlock();

            Controller::createBall(0.1f, 2.5f - rand () % 5, -1.f);
        }
    }
}

void Controller::keyboardHandler(int platformId, int socket_fd, int connection_fd, struct sockaddr_in client){
    for(ever){
        printf("Vou travar ate receber alguma coisa\n");
        char input_buffer[sizeof(NetworkMessage)];
        bool stop = false;
        int retVal = recv(connection_fd, input_buffer, 1, 0);
        if (retVal == 0) {
            cerr << "Warning: Received 0 byte" << endl;
        } else if (retVal < 0) {
            cerr << "ERROR: " << strerror(errno) << endl;
        } else {
            printf("Recebi uma mensagem: %s\n", input_buffer);

            Platform *platform = (Platform*)(Controller::gameObjects[platformId]);

            /* Identificando cliente */
            char ip_client[INET_ADDRSTRLEN];
            inet_ntop( AF_INET, &(client.sin_addr), ip_client, INET_ADDRSTRLEN );
            printf("IP que enviou: %s\n", ip_client);

            switch(input_buffer[0]) {
                //if a is pressed, the platform moves to the left
                case 'A':
                case 'a': {
                    platform->moveLeft();
                    break;
                }
                //if d is pressed, the platform moves to the right
                case 'D':
                case 'd': {
                    platform->moveRight();
                    break;
                }
                //if b is pressed, another ball appears in a random place (within a limiter of positions)
                case 'B':
                case 'b': {
                    Controller::createBall(0.1f, 2.5f - rand () % 5, -1.f);
                    break;
                }
                //if r is pressed, the creates all the bricks again (instatiate new objects)
                case 'R':
                case 'r': {
                    for (int i=-7; i<=7; i++) {
                        for (int j=0; j<5; j++) {
                            Controller::createBrick(i * .6f, 2.5f - j*.5f);
                        }
                    }
                    break;
                }
                case 'Q':
                case 'q': {
                    stop = true;
                    break;
                }
        
                default:
                    break;
            }
            if (stop)
                break;

            std::this_thread::sleep_for (std::chrono::milliseconds(1));
        }
    }
    close(socket_fd);
}

int Controller::createBall(float radius, float x, float y) {
    Ball* obj1 = new Ball(radius, x, y);
    Ball* obj2 = new Ball(radius, x, y);
    int id = obj1->setId();
    obj2->id = id;
    Controller::gameObjects[id] = obj1;
    Controller::balls[id] = obj2;
    return id;
}

int Controller::createBox(float x, float y, float z, float width, float height) {
    Box* obj = new Box(x, y, z, width, height);
    int id = obj->setId();
    Controller::gameObjects[id] = obj;
    return id;
}

int Controller::createBrick(float x, float y) {
    Brick* obj = new Brick(x, y);
    int id = obj->setId();
    obj->updateCollisionLogic();
    Controller::gameObjects[id] = obj;
    Controller::gameObjects[id]->destroyable = true;
    return id;
}

int Controller::createPlatform() {
    Platform* obj = new Platform();
    int id = obj->setId();
    Controller::gameObjects[id] = obj;
    return id;
}

void Controller::markForDeletion(int id) {
    gameObjects[id]->deleted = true;
    Controller::toBeDeleted.push_back(id);
}

void Controller::applyDeletions() {
    list<int>::iterator it1;
    for (it1 = Controller::toBeDeleted.begin(); it1 != Controller::toBeDeleted.end(); ++it1) {
        int index = *it1;
        map<int, GameObject*>::iterator it2;
        it2 = Controller::gameObjects.find(index);
        cout << "Deleting game object " << index << endl;
        Controller::sendDestroy(index);
        // Controller::gameObjects.erase(it2);
    }
    Controller::toBeDeleted.clear();
}

void Controller::messageSender() {
    for(ever) {
        NetworkMessage msg;

        bool hasMsg = false;
        Controller::msgQueueMtx.lock();
        if (!Controller::pendingMessages.empty()) {
            hasMsg = true;
            msg = Controller::pendingMessages.front();
            Controller::pendingMessages.pop();
        }
        Controller::msgQueueMtx.unlock();
        
        if (hasMsg) {
            Controller::receiverThreadsMtx.lock();
            map<int, thread>::iterator it;
            for(it = Controller::receiverThreads.begin(); it != Controller::receiverThreads.end(); ++it) {
                int socket_fd = it->first;
                Controller::sendMessage(msg, socket_fd);
            }
            Controller::receiverThreadsMtx.unlock();
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
    }
}

void Controller::sendMessage(NetworkMessage msg, int socket_fd) {
    auto retVal = send(socket_fd, (const void*)&msg, sizeof(msg), 0);
    if (retVal == 0) {
        cerr << "Warning: Send returned 0" << endl;   
    } else if (retVal == -1) {
        cerr << "ERROR: " << strerror(errno) << endl;
    } else {
        cout << "Sent " << retVal << " bytes" << endl; 
    }
}

void Controller::sendNewObject(GameObject newGameObject) {
    NetworkMessage msg;
    msg.messageType = (char)MessageType_NewObject;
    msg.objectId = newGameObject.getId();
    msg.rendererType  = (char)newGameObject.rendererType;
    msg.x = newGameObject.x;
    msg.y = newGameObject.y;
    msg.z = newGameObject.z;
    msg.r = newGameObject.r;
    msg.g = newGameObject.g;
    msg.b = newGameObject.b;
    msg.width = newGameObject.width;
    msg.height = newGameObject.height;
    msg.depth = newGameObject.depth;
    Controller::enqueueMessage(msg);

    Controller::permanentMsgQueueMtx.lock();
    msg.sequence = Controller::sequence;
    Controller::permanentMessages.push_back(msg);
    Controller::permanentMsgQueueMtx.unlock();
}

void Controller::sendNewPosition(GameObject gameObject) {
    Controller::sendNewPosition(gameObject.getId(), gameObject.x, gameObject.y, gameObject.z);
}

void Controller::sendNewPosition(int gameObjectId, float x, float y, float z) {
    NetworkMessage msg;
    msg.messageType = (char)MessageType_NewPosition;
    msg.objectId = gameObjectId;
    msg.x = x;
    msg.y = y;
    msg.z = z;
    cout << "Sending object " << msg.objectId << " position " << msg.x << " " << msg.y << " " << msg.z << endl;
    Controller::enqueueMessage(msg);
}

void Controller::sendDestroy(int id) {
    NetworkMessage msg;
    msg.messageType = (char)MessageType_Destroy;
    msg.objectId = id;
    Controller::enqueueMessage(msg);

    Controller::permanentMsgQueueMtx.lock();
    msg.sequence = Controller::sequence;
    Controller::permanentMessages.push_back(msg);
    Controller::permanentMsgQueueMtx.unlock();
}

void Controller::sendSound() {
    NetworkMessage msg;
    msg.messageType = (char)MessageType_PlaySound;
    Controller::enqueueMessage(msg);
}

#undef ever