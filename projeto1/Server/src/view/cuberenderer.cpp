#include <view/cuberenderer.hpp>
#include <view/glmanager.hpp>   

CubeRenderer::CubeRenderer() { 
    this->renderingFunction = GLManager::renderCube;
}
