#include <controller/controller.hpp>
#include <model/brick.hpp>
#include <cstdlib>
#include <ctime>

using namespace std;

Ball *Controller::ball;
Platform *Controller::platform = new Platform();

void Controller::init() {
    srand(time(NULL));
    Controller::ball = new Ball(0.1f, 2.5f - rand () % 5, -1.f);


	Box *ceiling = new Box(0.f, 3.5f, -8.f, 10.5f, .5f);
	Box *leftWall = new Box(-5.f, -1.5f, -8.f, .5f, 10.f);
	Box *rightWall = new Box(5.f, -1.5f, -8.f, .5f, 10.f);
    ceiling->renderer->r   = 0.4f; ceiling->renderer->g   = 0.4f; ceiling->renderer->b   = 0.4f;
    leftWall->renderer->r  = 0.4f; leftWall->renderer->g  = 0.4f; leftWall->renderer->b  = 0.4f;
    rightWall->renderer->r = 0.4f; rightWall->renderer->g = 0.4f; rightWall->renderer->b = 0.4f;

    for (int i=-7; i<=7; i++) {
        for (int j=0; j<5; j++) {
            Brick *brick = new Brick(i * .6f, 2.5f - j*.5f);
        }
    }
}

void Controller::update() {
    map<int, GameObject*>::iterator it1;
    for (it1 = GameObject::gameObjects.begin(); it1 != GameObject::gameObjects.end(); ++it1) {
        it1->second->update();
    }
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