#include <controller/controller.hpp>
#include <cstring>

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
    return 0;
}