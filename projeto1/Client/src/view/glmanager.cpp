#include <view/glmanager.hpp>
#include <controller/controller.hpp>
#include <iostream>

using namespace std;

bool GLManager::ready = false;

int Controller::score;

//this is the GLUT and OpenGL manager, it defines the position of lights, window,
//defines our mainloop, reshape function and most importantly
//defines that our idle function is the controller update, so
//we can follow MVC pattern.

void GLManager::init(int argc, char** argv, char name[]) {

    Controller::score = 0;
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
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(GLManager::glDisplay);       // Register callback handler for window re-paint event
    glutIdleFunc(GLManager::glIdle);
    glutReshapeFunc(GLManager::glReshape);       // Register callback handler for window re-size event
    glutKeyboardFunc(Controller::readKeyboardInput);
    GLManager::ready = true;
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

    if (!Controller::ended) {
        map<int, Renderer>::iterator it;
        for (it = Controller::renderers.begin(); it != Controller::renderers.end(); ++it) {
            it->second.render();
        }    
    } else {
        string gameOver = "GAME OVER";
        GLManager::renderText(gameOver, 0.f, 1.f, 0.f);
    }
    GLManager::renderText(to_string(Controller::score), 0.f, 2.2678f, 0.f);

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
    glutPostRedisplay();
}

void GLManager::renderCube() {
    glutSolidCube(1.f);
}

void GLManager::renderSphere() {
    glutSolidSphere(1.0f, 100, 100); // radius, slices, stacks
}

void GLManager::redisplay() {
    if (GLManager::ready)
        glutPostRedisplay();
}

void GLManager::mainLoop() {
	glutMainLoop();
}

void GLManager::exitGlut() {
    //glutLeaveMainLoop();
    //exit(1);
}

void GLManager::renderText(string txt, float x, float y, float z) {
    float scale = 0.002f;
    glPushMatrix();

    GLfloat mat[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    GLfloat shininess[] = { 100.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT,  mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);
    for(string::iterator it = txt.begin(); it != txt.end(); ++it) {
        glutStrokeCharacter( GLUT_STROKE_MONO_ROMAN , *it);
    }
    glPopMatrix();
}
