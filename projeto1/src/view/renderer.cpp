#include <view/renderer.hpp>

void Renderer::render() {}

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