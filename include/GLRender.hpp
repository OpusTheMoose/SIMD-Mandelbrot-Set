#include <math.h>
#include <SDL3/SDL.h>
#include "glad/gl.h"
#pragma once




class GLRender
{
    public:
        GLRender(unsigned int SCRN_WIDTH, unsigned int SCRN_HEIGHT);
        void Render(uint8_t *pixels);
        void Clear();
    private:
        unsigned int VAO;
        unsigned int texture;
        unsigned int texture_width;
        unsigned int texture_height;
};
