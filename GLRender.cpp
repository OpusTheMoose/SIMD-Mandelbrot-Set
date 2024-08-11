#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include "shader.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "GLRender.hpp"

GLRender::GLRender(unsigned int SCREEN_WIDTH, unsigned int SCREEN_HEIGHT)
{
        this->WIN_WIDTH = SCREEN_WIDTH;
        this->WIN_HEIGHT = SCREEN_HEIGHT;
        unsigned int VBO;

    
        if (!gladLoadGL())
        {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
        }
        
        Shader ourShader("vertexShader.vs", "fragShader.fs");
       
         float vertices[] = {
        // positions         // texture
        -1.0f,  -1.0f,    0.0f,  0.0f, //bottom-left
        -1.0f,  1.0f,     0.0f,  1.0f, //top-left
        1.0f,  -1.0f,     1.0f,  0.0f, //bottom-right
        -1.0f,  1.0f,     0.0f,  1.0f, //top-left
        1.0f,  1.0f,      1.0f,  1.0f, //top-right
        1.0f,  -1.0f,     1.0f,  0.0f //bottom-right
        }; 
    
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &VBO);
   
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        // position attribute
        glBindVertexArray(this->VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

   
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
      

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glGenerateMipmap(GL_TEXTURE_2D);
        glEnable(GL_MULTISAMPLE);

        ourShader.use();
}

void GLRender::Render(uint8_t *pixels)
{
     
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels );
   
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

            
     glDrawArrays(GL_TRIANGLES, 0, 6);
    return;
    
}
void GLRender::Clear()
{
    glDeleteVertexArrays(1, &VAO);
   
}