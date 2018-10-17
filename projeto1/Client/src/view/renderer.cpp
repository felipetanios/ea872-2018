#include <view/renderer.hpp>
#include <view/glmanager.hpp>

void Renderer::render() {
	if (this->renderingFunction != nullptr) {

	   	glPushMatrix(); 
	 
	    glLoadIdentity(); 
	    GLfloat mat[] = { this->r, this->g, this->b, 1.0f };
	    GLfloat shininess[] = { 100.0f };
	    glMaterialfv(GL_FRONT, GL_AMBIENT,  mat);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE,  mat);
	    glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
	    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	    glTranslatef(this->x, this->y, this->z);
	    glScalef(this->width, this->height, this->depth);
	    
		this->renderingFunction();

		glPopMatrix();
	}
}

void Renderer::setSize(float width, float height, float depth) {
    this->width = width;
    this->height = height;
    this->depth = depth;
}

void Renderer::setColor(float r, float g, float b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

void Renderer::setPosition(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}