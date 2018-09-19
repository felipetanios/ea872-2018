#include <controller/controller.hpp>

using namespace std;

list<GameObject> Controller::gameObjects = {};
list<Renderer*> Controller::renderers = {};
Ball Controller::ball = {};
Platform Controller::platform = {};

void Controller::init() {

	Controller::gameObjects.push_back(ball);
	Controller::gameObjects.push_back(platform);

	list<GameObject>::iterator it;
    for (it = Controller::gameObjects.begin(); it != Controller::gameObjects.end(); ++it) {
        Controller::renderers.push_back((*it).renderer);
    }
}

void Controller::update() {}

void Controller::readKeyboardInput(unsigned char key, int x, int y) {
    switch(key) {
        case 'a':
            platform.moveLeft();
            break;
        case 'd':
            platform.moveRight();
            break;
    }
}