
#include <glad/glad.h>
#include <iostream>
//#include "fractal.hpp"
#include "fractalV2.hpp"
#include "shader.hpp"
#include "GLRender.hpp"
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>


int main()
{
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Fractal", sf::Style::Default);
   
  
    window.setActive(true);
    window.setFramerateLimit(60);
    
   
    GLRender *renderer = new GLRender(WIN_WIDTH, WIN_HEIGHT);
    Fractal *fractal = new Fractal(WIN_WIDTH, WIN_HEIGHT);
    
    sf::Clock clock;
    float delta_time = 0.0;
    float last_frame = 0.0;
    float frame_rate = 0.0;
    float const FRAME_RATE = (1.0/60.0);
    std::string title = "test";
    

    bool click = true;
    int frame_count = 0;
  
    while (window.isOpen())
    {
       
        sf::Event event;
      
        while (window.pollEvent(event))
        {
             
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
              if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click)
            {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                //Mandelbrot::CalculatePoint(pos.x, pos.y, zoom);
                //fractal->selectPoint(pos.x, pos.y);
                click = true;
            }
            if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left)) && click)
            {
                click = false;
            }
           
          
        }
         
       
         float current_frame = clock.getElapsedTime().asSeconds();
        delta_time = current_frame - last_frame;
     
        uint8_t *pixels = fractal->RenderSetSIMD(); //Generates the pixels
        renderer->Render(pixels); //Sends it off to be rendered by OpenGL
            
        window.display();
        
            //Displays the FPS every 50 frames (can be changed)
            frame_count++;
            if (frame_count > 50)
            {
                window.setTitle("FPS: " + std::to_string(1 / delta_time));
               
                frame_count = 0;
            }
            last_frame = current_frame;
     
        
    }
   // Window::Clear();
   renderer->Clear();
   delete renderer;
   delete fractal;
    return 0;
}
