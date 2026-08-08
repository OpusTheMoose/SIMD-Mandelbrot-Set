#include <iostream>
#include <math.h>
#include "../include/shader.hpp"
#include "../include/GLRender.hpp"

GLRender::GLRender(unsigned int SCREEN_WIDTH, unsigned int SCREEN_HEIGHT)
{
        this->texture_width= SCREEN_WIDTH;
        this->texture_height = SCREEN_HEIGHT;
        unsigned int VBO;

    
        
        // It's relative to the build folder. Future me would not do this~!
        Shader ourShader("../src/vertexShader.vs", "../src/fragShader.fs");
       
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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->texture_width, this->texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
      

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glGenerateMipmap(GL_TEXTURE_2D);
        glEnable(GL_MULTISAMPLE);

        ourShader.use();
}

void GLRender::Render(uint8_t *pixels)
{
     
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->texture_width, this->texture_height, GL_RGB, GL_UNSIGNED_BYTE, pixels );
   
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
