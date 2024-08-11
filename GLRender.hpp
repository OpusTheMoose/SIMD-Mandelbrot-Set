#include <glad/glad.h>
#include <iostream>
#include <math.h>

#include "shader.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
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
        unsigned int WIN_WIDTH;
        unsigned int WIN_HEIGHT;
};
