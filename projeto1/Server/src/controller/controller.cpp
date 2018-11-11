#include <controller/controller.hpp>
#include <model/brick.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ever ;; 

using namespace std;

uint64_t get_now_ms() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

thread Controller::keyboardHandlerThread;
thread Controller::messageSenderThread;

queue<NetworkMessage> Controller::pendingMessages;
mutex Controller::msgQueueMtx;

map<int, GameObject> Controller::gameObjects;
list<int> Controller::toBeDeleted;

int Controller::sequence;

map<int, thread> Controller::connection;

void Controller::init() {
    int socket_fd, connection_fd;
    struct sockaddr_in myself, client;
    socklen_t client_size = (socklen_t)sizeof(client);
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
    connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);

    printf("Conectado");
    
    //instantiating ball, platform, bounderies of play enviroment, bricks to destroy, sound player, 
    //sound sample and the pointer to sound threads list
    srand(time(NULL));
    Controller::createBall(0.1f, 2.5f - rand () % 5, -1.f);

	Controller::createBox(0.f, 3.5f, -8.f, 10.5f, .5f);
	Controller::createBox(-5.f, -1.5f, -8.f, .5f, 10.f);
	Controller::createBox(5.f, -1.5f, -8.f, .5f, 10.f);

    for (int i=-7; i<=7; i++) {
        for (int j=0; j<5; j++) {
            Controller::createBrick(i * .6f, 2.5f - j*.5f);
        }
    }

    /*
    *Initiating keyboard server thread
    */
    std::thread newKeyboardThread(Controller::keyboardHandler, Controller::createPlatform(), socket_fd, connection_fd, client);
    std::thread newSenderThread(Controller::messageSender, socket_fd);
    (Controller::keyboardHandlerThread).swap(newKeyboardThread);
    (Controller::messageSenderThread).swap(newSenderThread);

    for(ever) Controller::update();
}


void Controller::update() {
    //update objects positions
    map<int, GameObject>::iterator it1;
    for (it1 = Controller::gameObjects.begin(); it1 != Controller::gameObjects.end(); ++it1) {
        it1->second.update();
    }

    Controller::applyDeletions();

    //detects collision and make sound
    // if (Controller::ball->collided == true){
    //     //when a collision is detected create a new thread that rouns a method to play the sound effect
    //     printf("Colided\n");
    //     Controller::ball->collided = false;
    // }
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
}

void Controller::sendMessage(NetworkMessage msg) {
    Controller::msgQueueMtx.lock();
    msg.sequence = ++Controller::sequence;
    Controller::pendingMessages.push(msg);
    Controller::msgQueueMtx.unlock();
}

void Controller::keyboardHandler(int platformId, int socket_fd, int connection_fd, struct sockaddr_in client){
    for(ever){
        printf("Vou travar ate receber alguma coisa\n");
        char input_buffer[sizeof(NetworkMessage)];
        recv(connection_fd, input_buffer, 1, 0);
        printf("Recebi uma mensagem: %s\n", input_buffer);

        GameObject platformGameObject = Controller::gameObjects[platformId];
        Platform *platform = (Platform*)&platformGameObject;

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
                //GLManager::exitGlut();
                break;
            }
    
            default:
                break;
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
    }
    close(socket_fd);
}

int Controller::createBall(float radius, float x, float y) {
    Ball obj(radius, x, y);
    int id = obj.setId();
    Controller::gameObjects[id] = obj;
    return id;
}

int Controller::createBox(float x, float y, float z, float width, float height) {
    Box obj(x, y, z, width, height);
    int id = obj.setId();
    Controller::gameObjects[id] = obj;
    return id;
}

int Controller::createBrick(float x, float y) {
    Brick obj(x, y);
    int id = obj.setId();
    Controller::gameObjects[id] = obj;
    return id;
}

int Controller::createPlatform() {
    Platform obj = Platform();
    int id = obj.setId();
    Controller::gameObjects[id] = obj;
    return id;
}

void Controller::markForDeletion(int id) {
    gameObjects[id].deleted = true;
    Controller::toBeDeleted.push_back(id);
}

void Controller::applyDeletions() {
    list<int>::iterator it1;
    for (it1 = Controller::toBeDeleted.begin(); it1 != Controller::toBeDeleted.end(); ++it1) {
        int index = *it1;
        map<int, GameObject>::iterator it2;
        it2 = Controller::gameObjects.find(index);
        Controller::gameObjects.erase(it2);
    }
    Controller::toBeDeleted.clear();
}

void Controller::messageSender(int socket_fd) {
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
            auto retVal = send(socket_fd, (const void*)&msg, sizeof(msg), 0);
            cout<<retVal<<endl;   
        }
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
    Controller::sendMessage(msg);
}

void Controller::sendNewPosition(GameObject gameObject) {
    NetworkMessage msg;
    msg.messageType = (char)MessageType_NewPosition;
    msg.objectId = gameObject.getId();
    msg.x = gameObject.x;
    msg.y = gameObject.y;
    msg.z = gameObject.z;
    Controller::sendMessage(msg);
}

void Controller::sendDestroy(GameObject GameObject) {
    NetworkMessage msg;
    msg.messageType = (char)MessageType_Destroy;
    msg.objectId = GameObject.getId();
    Controller::sendMessage(msg);
}

void Controller::sendSound() {
    NetworkMessage msg;
    msg.messageType = (char)MessageType_PlaySound;
    Controller::sendMessage(msg);
}

#undef ever