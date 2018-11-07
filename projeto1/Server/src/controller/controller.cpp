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

Ball *Controller::ball;
Platform *Controller::platform = new Platform();

thread Controller::keyboardThread;
thread Controller::messageSenderThread;

queue<NetworkMessage> Controller::pendingMessages;
mutex Controller::msgQueueMtx;

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


    
    //instantiating ball, platform, bounderies of play enviroment, bricks to destroy, sound player, 
    //sound sample and the pointer to sound threads list
    srand(time(NULL));
    Controller::ball = new Ball(0.1f, 2.5f - rand () % 5, -1.f);

	Box *ceiling = new Box(0.f, 3.5f, -8.f, 10.5f, .5f);
	Box *leftWall = new Box(-5.f, -1.5f, -8.f, .5f, 10.f);
	Box *rightWall = new Box(5.f, -1.5f, -8.f, .5f, 10.f);

    for (int i=-7; i<=7; i++) {
        for (int j=0; j<5; j++) {
            Brick *brick = new Brick(i * .6f, 2.5f - j*.5f);
        }
    }

    /*
    *Initiating keyboard server thread
    */
    std::thread newKeyboardThread(Controller::keyboardHandler, Controller::platform);
    std::thread newSenderThread(Controller::messageSender);
    (Controller::keyboardThread).swap(newKeyboardThread);
    (Controller::messageSenderThread).swap(newSenderThread);

    for(ever) Controller::update();
}

void Controller::update() {
    //update objects positions
    map<int, shared_ptr<GameObject>>::iterator it1;
    for (it1 = GameObject::gameObjects.begin(); it1 != GameObject::gameObjects.end(); ++it1) {
        it1->second->update();
    }

    GameObject::applyDeletions();

    //detects collision and make sound
    if (Controller::ball->collided == true){
        //when a collision is detected create a new thread that rouns a method to play the sound effect
        printf("Colided\n");
        Controller::ball->collided = false;
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
}



void Controller::sendMessage(NetworkMessage msg) {
    Controller::msgQueueMtx.lock();
    Controller::pendingMessages.push(msg);
    Controller::msgQueueMtx.unlock();
}



void Controller::keyboardHandler(Platform *platform){
    for(ever){
        printf("Vou travar ate receber alguma coisa\n");
        recv(connection_fd, input_buffer, 1, 0);
        printf("Recebi uma mensagem: %s\n", input_buffer);

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
                Ball *newBall = new Ball(0.1f, 2.5f - rand () % 5, -1.f);
                break;
            }
            //if r is pressed, the creates all the bricks again (instatiate new objects)
            case 'R':
            case 'r': {
                for (int i=-7; i<=7; i++) {
                    for (int j=0; j<5; j++) {
                        Brick *brick = new Brick(i * .6f, 2.5f - j*.5f);
                    }
                }
                break;
            }
            case 'Q':
            case 'q': {
                GLManager::exitGlut();
                break;
            }
    
            default:
                break;
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
    }
    close(socket_fd);
}

void Controller::messageSender(int socket_fd) {
    for(ever) {
        NetworkMessage msg;

        Controller::msgQueueMtx.lock();
        bool hasMsg = false;
        if (!Controller::pendingMessages.empty()) {
            hasMsg = true;
            msg = Controller::pendingMessages.front();
            Controller.pendingMessages.pop();
        }
        Controller::msgQueueMtx.unlock();
        
        if (hasMsg) {
            auto retVal = send(socket_fd, msg, sizeof(msg), 0);
            cout<<retVal<<endl;   
        }
    }
}

void Controller::sendNewObject(GameObject newGameObject) {
    NetworkMessage msg = {
        .messageType = (char)MessageType_NewObject,
        .objectId = newGameObject.getId(),
        .rendererType  = newGameObject.rendererType,
        .x = newGameObject.x,
        .y = newGameObject.y,
        .z = newGameObject.z,
        .r = newGameObject.r,
        .g = newGameObject.g,
        .b = newGameObject.b,
        .width = newGameObject.width,
        .height = newGameObject.height,
        .depth = newGameObject.depth,
    };
    Controller::sendMessage(msg);
}

#undef ever