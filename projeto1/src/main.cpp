#include <GL/glut.h>  
#include <model/platform.hpp> 
#include <view/renderer.hpp>
#include <list>

using namespace std;

/* Global variables */
char title[] = "3D Shapes";

list<Renderer*> renderers = {}; 

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glEnable(GL_DEPTH_TEST);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}
 
/* Handler for window-repaint event. Called back when the window first appears and
     whenever the window needs to be re-painted. */
void display() {
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
    for (it = renderers.begin(); it != renderers.end(); ++it) {
        (*it)->render();
    }
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
 
/* Handler for window re-size event. Called back when the window first appears and
     whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
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
 
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    Platform platform;
    renderers.push_back(platform.renderer);
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Enable double buffered mode
    glutInitWindowSize(640, 480);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    initGL();                       // Our own OpenGL initialization
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}