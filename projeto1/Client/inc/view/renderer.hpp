#ifndef RENDERER
#define RENDERER

typedef void (*RenderingFunction)(void);

class Renderer {
public:
    void setSize(float width, float height, float depth);
    void setColor(float r, float g, float b);
    void setPosition(float x, float y, float z);
    void render();
    float r = 1.f;
    float g = 1.f;
    float b = 1.f;
protected:
    float width = 1.f;
    float height = 1.f;
    float depth = 1.f;
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    RenderingFunction renderingFunction = nullptr;
};
#endif