/*
 * OGL01Shape3D.cpp: 3D Shapes
 */
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
 
/* Global variables */
char title[] = "3D Shapes";
 
/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}
 
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

   glPushMatrix(); 

   GLfloat light1pos[] = {5.0f, 10.0f, 5.0f, 1.0f};
   GLfloat light2pos[] = {1.0f, 1.0f, 1.0f, 1.0f};
   GLfloat light3pos[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightmodel[] =  {0.3f, 0.3f, 0.3f, 1.0f};

   glLightfv(GL_LIGHT0, GL_POSITION, light1pos);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light2pos);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light3pos);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightmodel);

   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
   glTranslatef(1.5f, -2.0f, -7.0f);  // Move right and into the screen
 
    GLfloat mat[] = { 1.0f, 0.f, 0.f, 1.0f };
    GLfloat shininess[] = { 100.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT,  mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glScalef(20.0f, 1.0f, 3.0f);
    glutSolidCube(0.2f);


   glPopMatrix();
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
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}
 
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
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