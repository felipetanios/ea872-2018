#include <view/glmanager.hpp> 
#include <controller/controller.hpp>

using namespace std;

/* Global variables */
char title[] = "Ballgame"; 

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    Controller::init();
    GLManager::init(argc, argv, title);
    GLManager::mainLoop();
    return 0;
}