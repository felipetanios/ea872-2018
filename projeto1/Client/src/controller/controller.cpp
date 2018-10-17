#include <controller/controller.hpp>
#include <view/glmanager.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdio.h>

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

mutex Controller::mtx;
queue<char> Controller::keyboardBuffer;
thread Controller::networkThread;


Sample *Controller::asample;
Player *Controller::player;


uint64_t get_now_ms() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

void Controller::init() {
    //instantiating ball, platform, bounderies of play enviroment, bricks to destroy, sound player, 
    //sound sample and the pointer to sound threads list
    srand(time(NULL));
   
    Controller::asample = new Sample();
    Controller::player = new Player();
    Controller::asample->load("assets/blip.dat");
    Controller::player->init();


    uint64_t t0, t1;
    while (1) {
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        t1 = get_now_ms();

        if (t1-t0 > 5000) break;
    }

    player->play(asample);
    player->pause();

    /*
    *Initiating keyboard server thread
    */
    thread newthread(networkHandler);
    (Controller::networkThread).swap(newthread);

}


//this is the thread created to play the sound effect
//it simply plays the sample object with the player object
void threadSound (Player *player, Sample *asample) {
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
}

void networkHandler(){
    int conection_opened;
    int socket_fd;
    struct sockaddr_in target;
    conection_opened = 1;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket criado\n");

    target.sin_family = AF_INET;
    target.sin_port = htons(3001);
    inet_aton("127.0.0.1", & (target.sin_addr));
    printf("Tentando conectar\n");
    if (connect(socket_fd, (struct sockaddr * ) & target, sizeof(target)) != 0) {
        printf("Problemas na conexao\n");
        return;
    }

    printf("Conectei ao servidor\n");

    printf("fim da configuarcao\n");

    
    while (1){
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
        auto retVal = send(socket_fd, msg, 1, 0);
        cout<<retVal<<endl;
        // printf("Escrevi mensagem de ping!\n");
        //sleep(1);

        /* Recebendo resposta */
        //char reply[10];
        //recv(socket_fd, reply, 10, 0);
        //printf("Resposta:\n%s\n", reply);
        std::this_thread::sleep_for (std::chrono::milliseconds(1));

    }
    close(socket_fd);

}


void Controller::readKeyboardInput(unsigned char key, int x, int y) {
    //read keyboard inputs that controlls the platform, create more balls and more brics to break
    Controller::mtx.lock();
    Controller::keyboardBuffer.push(key);
    Controller::mtx.unlock();
}