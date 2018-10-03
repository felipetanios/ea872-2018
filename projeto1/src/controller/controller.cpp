#include <controller/controller.hpp>
#include <model/brick.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>


using namespace std;

uint64_t get_now_ms() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}


Ball *Controller::ball;
Platform *Controller::platform = new Platform();
list<thread> Controller::soundThreads = {};


Sample *Controller::asample;
Player *Controller::player;

void Controller::init() {
    //instantiating ball, platform, bounderies of play enviroment, bricks to destroy, sound player, 
    //sound sample and the pointer to sound threads list
    srand(time(NULL));
    Controller::ball = new Ball(0.1f, 2.5f - rand () % 5, -1.f);

	Box *ceiling = new Box(0.f, 3.5f, -8.f, 10.5f, .5f);
	Box *leftWall = new Box(-5.f, -1.5f, -8.f, .5f, 10.f);
	Box *rightWall = new Box(5.f, -1.5f, -8.f, .5f, 10.f);
    ceiling->renderer.r   = 0.4f; ceiling->renderer.g   = 0.4f; ceiling->renderer.b   = 0.4f;
    leftWall->renderer.r  = 0.4f; leftWall->renderer.g  = 0.4f; leftWall->renderer.b  = 0.4f;
    rightWall->renderer.r = 0.4f; rightWall->renderer.g = 0.4f; rightWall->renderer.b = 0.4f;

    for (int i=-7; i<=7; i++) {
        for (int j=0; j<5; j++) {
            Brick *brick = new Brick(i * .6f, 2.5f - j*.5f);
        }
    }

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
        Controller::soundThreads.push_back(std::thread(threadSound, Controller::player, Controller::asample));
        printf("Colided\n");
        Controller::ball->collided = false;
    }
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

void Controller::readKeyboardInput(unsigned char key, int x, int y) {
    //read keyboard inputs that controlls the platform, create more balls and more brics to break
    switch(key) {
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
        // case 'Q':
        // case 'q': {
        //     glutLeaveMainLoop();
        //     break;
        // }

        default:
            break;
    }
}