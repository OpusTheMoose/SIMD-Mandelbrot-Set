#include <iostream>

#pragma once

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

class Fractal
{
    public:
        //Initalizes width, height, coords, and reads from settings.txt
        Fractal(unsigned int SCRN_WIDTH, unsigned int SCRN_HEIGHT);
        //Main algorithm for generating the mandelbrot set
         uint8_t* RenderSet();
         //void selectPoint(int x, int y);
         //Render the set using SIMD instructions
        uint8_t* RenderSetSIMD();
        inline void Plot(unsigned int i, float r, float g, float b);

        ~Fractal();

    private:
        unsigned int WIDTH, HEIGHT;
        unsigned static const int MAX_ITERS = 800;
        unsigned int frame_count = 0;
        
        double *coords;
        double zoom =  2.0;
        double center_x = 1.0, center_y = 0.0;

        uint8_t *pixels;
        bool enableZoom = true;
     
       
        
};

