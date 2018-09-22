#include <controller/controller.hpp>
#include <model/box.hpp>

using namespace std;

list<GameObject*> Controller::gameObjects = {};
list<Renderer*> Controller::renderers = {};
Ball *Controller::ball = new Ball(0.1f);
Platform *Controller::platform = new Platform();

void Controller::init() {

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