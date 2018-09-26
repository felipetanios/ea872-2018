#include <controller/controller.hpp>
#include <model/box.hpp>
#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>


using namespace std;

uint64_t get_now_ms() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

list<GameObject*> Controller::gameObjects = {};
list<Renderer*> Controller::renderers = {};
Ball *Controller::ball = new Ball(0.1f);
Platform *Controller::platform = new Platform();
list<thread> Controller::soundThreads = {};


//init audio
//Audio::Sample *Controller::asample;
Sample *Controller::asample;

//Audio::Player *Controller::player;
Player *Controller::player;

void Controller::init() {
    Controller::asample = new Sample();
    Controller::player = new Player();
    Controller::asample->load("assets/blip.dat");
    Controller::player->init();
    Controller::player->pause();
	Controller::gameObjects.push_back(ball);
	Controller::gameObjects.push_back(platform);

	// add limits
	Box *ceiling = new Box(0.f, 3.5f, -8.f, 10.f, .5f);
	Box *leftWall = new Box(-5.f, -1.5f, -8.f, .5f, 10.f);
	Box *rightWall = new Box(5.f, -1.5f, -8.f, .5f, 10.f);
	Controller::gameObjects.push_back(ceiling);
	Controller::gameObjects.push_back(leftWall);
	Controller::gameObjects.push_back(rightWall);

	list<GameObject*>::iterator it;
    for (it = Controller::gameObjects.begin(); it != Controller::gameObjects.end(); ++it) {
        Controller::renderers.push_back((*it)->renderer);
    }
}

void Controller::update() {
    list<GameObject*>::iterator it;
    for (it = Controller::gameObjects.begin(); it != Controller::gameObjects.end(); ++it) {
        (*it)->update();
    }

    //detects collision and make sound
    if (Controller::ball->collided == true){
        Controller::soundThreads.push_back(std::thread(threadSound, Controller::player, Controller::asample));
        printf("Colided\n");
        Controller::ball->collided = false;
    }
}

void threadSound (Player *player, Sample *asample) {

    uint64_t t0, t1;

    asample->set_position(0);
    player->play(asample);

    t0 = get_now_ms();
    while (1) {
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        t1 = get_now_ms();

        if (t1-t0 > 500) break;
    }
    player->pause();
    //Controller::asample->set_position(0);
    // player->stop();
}

void Controller::readKeyboardInput(unsigned char key, int x, int y) {
    switch(key) {
        case 'a':
            platform->moveLeft();
            break;
        case 'd':
            platform->moveRight();
            break;
    }
}