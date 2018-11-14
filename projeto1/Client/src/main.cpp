#include <view/glmanager.hpp> 
#include <controller/controller.hpp>
#include <cstring>

using namespace std;

/* Global variables */
char title[] = "BRICK"; 

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	char serverIp[16] = "127.0.0.1";
	int serverPort = 3001;
	if (argc > 1) {
		strcpy(serverIp, argv[1]);
	}
	if (argc > 2) {
		serverPort = stoi(argv[2]);
	}
    Controller::init(serverIp, serverPort);
    GLManager::init(argc, argv, title);
    GLManager::mainLoop();
    return 0;
}