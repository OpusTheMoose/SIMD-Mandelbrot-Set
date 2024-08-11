#include <cmath>
#include <omp.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <immintrin.h>
#include <xmmintrin.h>
#include "fractalV2.hpp"
#include "colorConverter.hpp"


Fractal::Fractal(unsigned int SCRN_WIDTH, unsigned int SCRN_HEIGHT)
{
    WIDTH = SCRN_WIDTH;
    HEIGHT = SCRN_HEIGHT;
   
    coords = new double[WIDTH + HEIGHT];
    pixels = new uint8_t[WIDTH * HEIGHT * 3];

     //Pre-compute inital values of x and y, so it's scaled to lie in the plane
    for (int x = 0; x < WIDTH; x++)
    {
        coords[x] = x / static_cast<float>(WIDTH) - 0.5;
    }
    
    for (int y = 0; y < HEIGHT; y++)
    {
        coords[y + WIDTH] = (y / static_cast<float>(WIDTH) - 0.3);
    }

    std::ifstream settings("settings.txt");
    if (settings.fail())
    {
        std::cout << "ERROR: FAILED TO OPEN SETTINGS" << std::endl;
        return;
    }
    std::string var;
    std::stringstream s(var);
    //Write the file's read buffer into the stringstream.
    s << settings.rdbuf();
    //Close the file, since its buffer is written into stringstream
    settings.close();
    while (s)
    {
        std::string var_name;
        double val;
        s >> var_name >> val;
        if (var_name == "centerX:")
        {
            this->center_x = val;

        }
        else if (var_name == "centerY:")
        {
            this->center_y = val;
        }
        else if (var_name == "initalZoom:")
        {
            this->zoom = val;
        }
        else if (var_name == "enableZoom:")
        {
            this->enableZoom = val;
        }
    }
    
    

}
// void Fractal::selectPoint(int x, int y)
// {
//     double x0 = (coords[x] * zoom) + center_x;
//     double y0 = (coords[y + WIDTH] * zoom) - center_y;
//     center_x = x0;
//     center_y = y0;
// }
uint8_t* Fractal::RenderSet()
{
    //Copy private member variables to local static variables for faster lookup times
    static const unsigned int SCRN_WIDTH = this->WIDTH;
    static const unsigned int SCRN_HEIGHT = this->HEIGHT;

    static const double CENTER_X = this->center_x;
    static const double CENTER_Y = this->center_y;
    const double ZOOM = this->zoom;
 
    //#pragma omp parallel for simd
   
    for (int i = 0; i < SCRN_WIDTH * SCRN_HEIGHT; ++i)
    {
        const double x0 = (coords[i % SCRN_WIDTH] * ZOOM) + CENTER_X;
        const double y0 = (coords[(i / SCRN_WIDTH) + SCRN_WIDTH] * ZOOM) + CENTER_Y;

     
        double x1 = 0;
        double y1 = 0;
        double x2 = 0;
        double y2 = 0;
      
        int iter = 0;
        double zn = 0;

        double old_zn = 0;
        //Main escape time algorithm
        do
        {
          y1 = 2 * x1 * y1 + y0;
          x1 = x2 - y2 + x0;
        //   __m256d op1 = _mm256_set_pd(0.0, 0.0, x1, y1);
        //   __m256d op2 = _mm256_set_pd(0.0, 0.0, x1, y1);
        //   __m256d result = _mm256_mul_pd(op1, op2);
          
        //   zn = result[0] + result[1];
        //   x2 = result[1];
        //   y2 = result[0];
          x2 = x1 * x1;
          y2 = y1 * y1;
          zn = x2 + y2;

          //Period checker
        //   if (fabs(zn - old_zn) < 0.0000001)
        //   {
        //     iter = MAX_ITERS;
        //   }
        
        //   old_zn = zn;
         
          iter++;
        } while (zn <= 4 && iter < MAX_ITERS );
       
       
        
    

        float r = 0.0;
        float g = 0.0;
        float b = 0.0;
        //Smooth coloring algorithm
        //Only applies color if iterations < max iters, otherwise color is 0.
       if (iter < MAX_ITERS)
       {
        
           float nsmooth = iter + 1 - log(log(zn)) / log(2);
         
          float hue = 0.95 + 10 * nsmooth;
           static float saturation = 0.6;
           static float value = 1.0;
          
        
         
                HSVtoRGB(r, g, b, hue, saturation, value);
         
                r *= 255;
                g *= 255;
                b *= 255;
                // std::cout << "r: " << r << std::endl;
                // std::cout << "g: " << r << std::endl;
                // std::cout << "b: " << r << std::endl;
            
             
           
        }
        
     
        //Places the pixel at index i
        Plot(i, r, g, b);
        
    }
    if (enableZoom)  zoom = exp(-0.01 * frame_count);  
   
    frame_count++;
    return pixels;
}
uint8_t* Fractal::RenderSetSIMD()
{
    //Copy private member variables to local static variables for faster lookup times
    static const unsigned int SCRN_WIDTH = this->WIDTH;
    static const unsigned int SCRN_HEIGHT = this->HEIGHT;

    static const double CENTER_X = this->center_x;
    static const double CENTER_Y = this->center_y;
    const double ZOOM = this->zoom;

    static const __m256i ONE = _mm256_set_epi64x(1, 1, 1, 1);
    static const __m256d TWOF = _mm256_set1_pd(2.0);
    static const __m256d ESCAPE_RADIUS = _mm256_set1_pd(255.0);

    #pragma omp parallel for
    for (int i = 0; i < (SCRN_WIDTH * SCRN_HEIGHT) ; i += 4)
    {
        // const double x0 = (coords[i % SCRN_WIDTH] * ZOOM) + CENTER_X;
        const __m256d x0 = _mm256_set_pd(
            (coords[(i + 3) % SCRN_WIDTH] * ZOOM) + CENTER_X,
            (coords[(i + 2) % SCRN_WIDTH] * ZOOM) + CENTER_X,
            (coords[(i + 1) % SCRN_WIDTH] * ZOOM) + CENTER_X,
            (coords[i % SCRN_WIDTH] * ZOOM) + CENTER_X
        );
    
        // const double y0 = (coords[(i / SCRN_WIDTH) + SCRN_WIDTH] * ZOOM) + CENTER_Y;
        const __m256d y0 = _mm256_set_pd(
            (coords[(i / SCRN_WIDTH) + SCRN_WIDTH] * ZOOM) + CENTER_Y,
            (coords[((i + 1) / SCRN_WIDTH) + SCRN_WIDTH] * ZOOM) + CENTER_Y,
            (coords[((i + 2) / SCRN_WIDTH) + SCRN_WIDTH] * ZOOM) + CENTER_Y,
            (coords[((i + 3) / SCRN_WIDTH) + SCRN_WIDTH] * ZOOM) + CENTER_Y
        );

        //Initalize vectors for escape time algorithm.                    
        __m256d x = _mm256_setzero_pd();
        __m256d y = _mm256_setzero_pd();
        __m256i iters = _mm256_setzero_si256();
        __m256d zn = _mm256_setzero_pd();
        __m256d mask = _mm256_castsi256_pd(_mm256_set1_epi64x(-1)); //sets field to 1
        
        int iter = 0; //Keeps track of ALL iters. It's the final "bailout"
       
         //Main escape time algorithm
        do
        {
          //double xtemp = x*x - y*y + x0;
          __m256d xtemp = _mm256_add_pd(_mm256_sub_pd(_mm256_mul_pd(x, x), _mm256_mul_pd(y, y)), x0);
          
          //y = 2*x*y + y0;
          y = _mm256_add_pd(_mm256_mul_pd(_mm256_mul_pd(x, y), TWOF), y0);
          //Previous function uses x, so we didnt want to update the actual value of x until now.
          x = xtemp;
          //zn = x*x + y*y
         //Blend zn with the invert of the mask, so we can stop calculating values of zn when we escape
          zn = _mm256_blendv_pd(_mm256_add_pd(_mm256_mul_pd(x, x), _mm256_mul_pd(y, y)), zn, _mm256_xor_pd(mask, _mm256_set1_pd(-1.0)));
         
           //zn > 4
           __m256d cmp = _mm256_cmp_pd(zn, ESCAPE_RADIUS, _CMP_LT_OQ);
           //Creates a new mask. If a cmp lane is set to 0, set the corresponding mask lane to 0.
          mask = _mm256_and_pd(mask, cmp);
       
          __m256i plus_one = _mm256_add_epi64(iters, ONE);
          iters = _mm256_blendv_epi8(iters, plus_one, _mm256_castpd_si256(mask));
          iter++;
        
        } while (_mm256_movemask_pd(mask) != 0x0 && iter < MAX_ITERS);
  
        //Initalize values of r, g, b
        __m256 r = _mm256_setzero_ps();
        __m256 g = _mm256_setzero_ps();
        __m256 b = _mm256_setzero_ps();
        __m256 nsmooth = _mm256_setzero_ps();
      
        /*  TODO: Implement SVML for intel and similar library for AMD */
        nsmooth[0] = iters[0] + 1 - log(log(zn[0])) / log(2);
        nsmooth[1] = iters[1] + 1 - log(log(zn[1])) / log(2);
        nsmooth[2] = iters[2] + 1 - log(log(zn[2])) / log(2);
        nsmooth[3] = iters[3] + 1 - log(log(zn[3])) / log(2);

        //For setting value to 0 when MAX_ITERS is hit
        __m256i max_iters_mask = _mm256_cmpeq_epi64(iters, _mm256_set1_epi64x(MAX_ITERS));
        
       __m256 hue = _mm256_add_ps(_mm256_set1_ps(0.95), _mm256_mul_ps(_mm256_set1_ps(10.0), nsmooth));
        __m256 saturation = _mm256_set1_ps(0.5);
        __m256i value = _mm256_set1_epi64x(1);
        //Trying to set points that hit max_iters to black. Needs work
        value = _mm256_blendv_epi8(value, _mm256_setzero_si256(), max_iters_mask);
        __m256 new_value;
        new_value[0] = value[0];
        new_value[1] = value[1];
        new_value[2] = value[2];
        new_value[3] = value[3];


        HSVtoRGBSIMD(r, g, b, hue, saturation, new_value);
        //rgb values are return as a value between 0-1
        r = _mm256_mul_ps(r, _mm256_set1_ps(255.0));
        g = _mm256_mul_ps(g, _mm256_set1_ps(255.0));
        b = _mm256_mul_ps(b, _mm256_set1_ps(255.0));
        
         

        Plot(i, r[0], g[0], b[0]);
        Plot(i + 1, r[1], g[1], b[1]);
        Plot(i + 2, r[2], g[2], b[2]);
        Plot(i + 3, r[3], g[3], b[3]);
  
        
    }
 
  if (enableZoom) zoom = exp(-0.01 * frame_count);  
    
    frame_count++;
    return pixels;
}

inline void Fractal::Plot(unsigned int i, float r, float g, float b)
{
    //i *=3 gets the pixel position, since we need RGB elements
    i *= 3;
    pixels[i] = r;
    pixels[i + 1] = g;
    pixels[i + 2] = b;
   
}

Fractal::~Fractal()
{
    delete coords;
    delete pixels;
}