#include <view/sphererenderer.hpp>
#include <view/glmanager.hpp>  

SphereRenderer::SphereRenderer() { 
    this->renderingFunction = GLManager::renderSphere;
}
