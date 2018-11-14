#ifndef EA872_GL_MANAGER
#define EA872_GL_MANAGER

#ifdef TARGET_OS_MAC
    #include <GLUT/glut.h> 
#endif
#ifdef __linux__
	#include <GL/glut.h>
#endif

#include <string>

using namespace std;

class GLManager {
public:
	static void init(int argc, char** argv, char name[]);
	static void glDisplay();
	static void glReshape(GLsizei width, GLsizei height);
	static void glIdle();
	static void renderSphere();
	static void renderCube();
	static void redisplay();
	static void mainLoop();
	static void exitGlut();
	static void renderText(string txt, float x, float y, float z);
	static bool ready;
};
#endif