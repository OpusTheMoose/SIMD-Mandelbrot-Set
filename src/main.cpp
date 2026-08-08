#include "../include/glad/gl.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include "../include/fractalV2.hpp"
#include "../include/shader.hpp"
#include "../include/GLRender.hpp"
#include <math.h>



int main()
{
        

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == false)
    { 
        return -1;

    }
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);  
    SDL_Window* window = SDL_CreateWindow("Mandelbrot Set", 800, 600, SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) return -1;

 
    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
 

    //window.setFramerateLimit(60);


    int dim_w, dim_h;
    SDL_GetWindowSize(window, &dim_w, &dim_h);
   
    GLRender *renderer = new GLRender(dim_w, dim_h);
    Fractal *fractal = new Fractal(dim_w, dim_h);
    
    SDL_Time clock;
    float delta_time = 0.0;
    float last_frame = 0.0;
    float frame_rate = 0.0;
    float const FRAME_RATE = (1.0/60.0);


    bool click = true;
    int frame_count = 0;

    bool window_open = true;
  
    while (window_open)
    {
       
        SDL_Event event;
      
        while (SDL_PollEvent(&event))
        {
             
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                window_open = false;
            // else if (event.type == sf::Event::Resized)
            // {
            //     // adjust the viewport when the window is resized
            //     glViewport(0, 0, event.size.width, event.size.height);
            // }
            //   if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click)
            // {
            //     sf::Vector2i pos = sf::Mouse::getPosition(window);
            //     //Mandelbrot::CalculatePoint(pos.x, pos.y, zoom);
            //     //fractal->selectPoint(pos.x, pos.y);
            //     click = true;
            // }
            // if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left)) && click)
            // {
            //     click = false;
            // }
           
          
        }
         
       
        //  float current_frame = clock.getElapsedTime().asSeconds();
        // delta_time = current_frame - last_frame;
     
        uint8_t *pixels = fractal->RenderSetSIMD(); //Generates the pixels
        renderer->Render(pixels); //Sends it off to be rendered by OpenGL
            
            //Displays the FPS every 50 frames (can be changed)
            frame_count++;
            if (frame_count > 50)
            {
                SDL_SetWindowTitle(window, ("FPS: " + std::to_string(1 / delta_time)).c_str());
               
                frame_count = 0;
            }
        //    last_frame = current_frame;
        SDL_GL_SwapWindow(window);
     
        
    }
   SDL_DestroyWindow(window);
   SDL_Quit();
   renderer->Clear();
   delete renderer;
   delete fractal;
    return 0;
}
