#include "Renderer.hpp"

class TextRenderer : public Renderer {
public:
    TextRenderer(Shader *shader, Texture **textures = NULL, Light *light = NULL);
    ~TextRenderer();

    void draw();
private:
    void initial();
};