#include <view/glmanager.hpp>
#include <controller/controller.hpp>

using namespace std;

list<Renderer*> GLManager::renderers = {};

void GLManager::init(int argc, char** argv, char name[]) {
	renderers = Controller::renderers;
	glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Enable double buffered mode
    glutInitWindowSize(640, 480);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50);
    glutCreateWindow(name); 
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glEnable(GL_DEPTH_TEST);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections




    glutDisplayFunc(GLManager::glDisplay);       // Register callback handler for window re-paint event
    glutIdleFunc(GLManager::glIdle);
    glutReshapeFunc(GLManager::glReshape);       // Register callback handler for window re-size event
    glutKeyboardFunc(Controller::readKeyboardInput);
}

void GLManager::glDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

    GLfloat light1pos[] = {5.0f, 10.0f, 5.0f, 1.0f};
    GLfloat light2pos[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light3pos[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightmodel[] =  {0.3f, 0.3f, 0.3f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light1pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light2pos);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light3pos);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightmodel);

    list<Renderer*>::iterator it;
    for (it = Controller::renderers.begin(); it != Controller::renderers.end(); ++it) {
        (*it)->render();
    }
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void GLManager::glReshape(GLsizei width, GLsizei height) {
	// GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(60.0f, aspect, 0.1f, 20.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void GLManager::glIdle() {
    Controller::update();
    glutPostRedisplay();
}

void GLManager::redisplay() {
    glutPostRedisplay();
}

void GLManager::mainLoop() {
	glutMainLoop();
}
