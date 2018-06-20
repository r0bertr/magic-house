#include FT_FREETYPE_H
#include <freetype2/ft2build.h>
#include "Renderer.hpp"

class TextRenderer : public Renderer {
public:
    FT_Library library; // 声明了Lib

    TextRenderer(Shader *shader, Texture **textures = NULL, Light *light = NULL);
    ~TextRenderer();

    void draw()
private:
    void initial();
}