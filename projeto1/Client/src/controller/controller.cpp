#include <controller/controller.hpp>
#include <view/glmanager.hpp>
#include <view/cuberenderer.hpp>
#include <view/sphererenderer.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstring>
#include <cerrno>

#include <chrono>
#include <iostream>
#include <thread>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;

#define ever ;;

mutex Controller::mtx;
queue<char> Controller::keyboardBuffer;

Sample *Controller::asample;
Player *Controller::player;

int Controller::socketId;
int Controller::connectionId;
thread Controller::senderThread;
thread Controller::receiverThread;
list<thread> Controller::soundThreads;

map<int, Renderer> Controller::renderers;

uint64_t get_now_ms() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

void Controller::init() {
    // init sound 
    Controller::asample = new Sample();
    Controller::player = new Player();
    Controller::asample->load("assets/blip.dat");
    Controller::player->init();

    uint64_t t0, t1;
    for(ever) {
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        t1 = get_now_ms();

        if (t1-t0 > 5000) break;
    }
    player->play(asample);
    player->pause();    

    // init connection
    int conection_opened;
    struct sockaddr_in target;
    conection_opened = 1;

    Controller::socketId = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket criado\n");

    target.sin_family = AF_INET;
    target.sin_port = htons(3001);
    inet_aton("127.0.0.1", & (target.sin_addr));
    while (connect(Controller::socketId, (struct sockaddr * ) & target, sizeof(target)) != 0) {
        printf("Tentando conectar\n");
        std::this_thread::sleep_for (std::chrono::milliseconds(500));
    }

    printf("Conectei ao servidor\n");

    senderThread = thread(senderLoop);
    receiverThread = thread(receiverLoop);
}

void Controller::receiverLoop() {
    cout << "Iniciando thread de recepção" << endl;
    for(ever) {
        NetworkMessage msg;
        int status = recv(Controller::socketId, &msg, sizeof(msg), 0);
        if (status < 0) {
            cerr << "ERROR: " << strerror(errno) << endl;
            return;
        }
        int msgType = (int)msg.messageType;
        switch (msgType) {
            
            case MessageType_NewObject: 
                {
                    cout << "New object " << msg.objectId << endl;
                    int rendType = (int)msg.rendererType;
                    Renderer newRenderer;
                    switch (rendType) {
                        case RendererType_Cube:
                            newRenderer = CubeRenderer();
                            break;
                        case RendererType_Sphere:
                            newRenderer = SphereRenderer();
                            break;
                    }
                    newRenderer.setSize(msg.width, msg.height, msg.depth);
                    newRenderer.setColor(msg.r, msg.g, msg.b);
                    newRenderer.setPosition(msg.x, msg.y, msg.z);
                    Controller::renderers[msg.objectId] = newRenderer;
                    GLManager::redisplay();
                }
                break;
            
            case MessageType_NewPosition:
                cout << "New object " << msg.objectId << " position " << msg.x << " " << msg.y << " " << msg.z << endl;
                Controller::renderers[msg.objectId].setPosition(msg.x, msg.y, msg.z);
                GLManager::redisplay();
                break;

            case MessageType_Destroy:
                cout << "Destroy object " << msg.objectId << endl;
                Controller::renderers.erase(msg.objectId);
                GLManager::redisplay();
                break;

            case MessageType_PlaySound:
                cout << "Play sound" << endl;
                Controller::soundThreads.push_back(std::thread(threadSound, Controller::player, Controller::asample));
                break;
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(1));
    }
    cout << "Finalizano thread de recepção" << endl;
}


//this is the thread created to play the sound effect
//it simply plays the sample object with the player object
void Controller::threadSound (Player *player, Sample *asample) {
    cout << "Iniciando thread de som" << endl;
    uint64_t t0, t1;
    //it starts the sample to the beggining position then plays it, wait a little and finally pauses the player
    asample->set_position(0);
    player->play(asample);

    t0 = get_now_ms();
    while (1) {
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        t1 = get_now_ms();

        if (t1-t0 > 500) break;
    }
    player->pause();
    cout << "Finalizando thread de som" << endl;
}

void Controller::senderLoop() {
    cout << "Iniciando thread de envio" << endl;
    for(ever) {
        char c = -1;
        Controller::mtx.lock();
        if (!Controller::keyboardBuffer.empty()){
            c = Controller::keyboardBuffer.front();
            Controller::keyboardBuffer.pop();
        }
        Controller::mtx.unlock();

        char msg[2] = "\0";


        if (c == -1)
            continue;
        
        msg[0] = c;

        // printf("essa é a entrada: %c, %d\n", c, c);

        if (c == 'q' || c == 'Q') {
            break;
        }

        printf("mensagem: %s\n", msg);
        /* Agora, meu socket funciona como um descritor de arquivo usual */
        auto retVal = send(Controller::socketId, msg, 1, 0);
        cout<<retVal<<endl;
        // printf("Escrevi mensagem de ping!\n");
        //sleep(1);

        /* Recebendo resposta */
        //char reply[10];
        //recv(Controller::socketId, reply, 10, 0);
        //printf("Resposta:\n%s\n", reply);
        std::this_thread::sleep_for (std::chrono::milliseconds(1));

    }
    close(Controller::socketId);
    cout << "Finalizando thread de envio" << endl;
}


void Controller::readKeyboardInput(unsigned char key, int x, int y) {
    //read keyboard inputs that controlls the platform, create more balls and more brics to break
    Controller::mtx.lock();
    Controller::keyboardBuffer.push(key);
    Controller::mtx.unlock();
}

#undef ever