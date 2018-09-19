#include <view/cuberenderer.hpp>
#include <glutheaders.hpp>  

SphereRenderer::SphereRenderer() { }

void SphereRenderer::render() {	
   	glPushMatrix(); 
 
    glLoadIdentity(); 
    GLfloat mat[] = { this->r, this->g, this->b, 1.0f };
    GLfloat shininess[] = { 100.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT,  mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    glTranslatef(this->x, this->y, this->z);
    // glScalef(this->width, this->height, this->depth);
    
    //void glutSolidSphere(GLdouble radius,
    //                 GLint slices, GLint stacks);
    glutSolidSphere(0.1f, 100, 100);

    glPopMatrix();
}