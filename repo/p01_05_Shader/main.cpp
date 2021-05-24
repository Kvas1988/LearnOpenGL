// www.docs.gl

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>

#include "shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// Shaders
const GLchar* vertexShaderSource = "#version 330 core \n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main() {\n"
"gl_Position = vec4(position, 1.0);\n"
"ourColor = color;\n"
"}\n\0";

const GLchar* fragmentShaderSource = "#version 330 core \n"
"out vec4 color;\n"
"in vec3 ourColor;\n"
"void main() {\n"
"color = vec4(ourColor,1.0f);\n"
"}\n\0";

int main()
{
    // GLFW init
    
    glfwInit();
    
    // Setup GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // set major version 3.#
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // set minor verison #.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // set profile
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // resizeable window turn off
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for Mac Os X
//    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE); // stop red flickering
    
    // Create Window
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr); // pointer on GLFWwindow object
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // Viewport
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    // GLEW init
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to init GLEW" << std::endl;
        return -1;
    }
    
    // callbacks
    glfwSetKeyCallback(window, key_callback);
    
    // Shaders
    Shader ourShader("default.vs", "default.frag");
    
    
    // triangle
    GLfloat vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
    };

    
//    GLuint indices[] = {
//        0, 1, 3, // 1st triangle
//        1, 2, 3 //  2nd one
//    };
    GLuint VBO; // Vertex Buffer Object
    GLuint VAO; // Vertex Array Object
    // GLuint EBO; // Element Buffer Object - for index drawing
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // bind VAO
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind gl_array_buffer with vbo
    // copy vbo data to gl_array_buffer
    // https://docs.gl/gl3/glBufferData
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
    // define an array of generic vertex attribute data
    // https://docs.gl/gl3/glVertexAttribPointer
    // GLvoid* is kinda offset
    // coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0 );
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0); // unbind VAO
    
    // wireframe polygons
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Game loop
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // get events (keys/mouse etc.)
        
        glClearColor(0.2f, 0.3f, 0.3f, 10.f);
        glClear(GL_COLOR_BUFFER_BIT); // fix for red flickering
        
        // Shader's part
        // glUseProgram(shaderProgram); // use shader program
        ourShader.Use();
        
//        GLfloat timeValue = glfwGetTime();
//        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
//        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        // vertex data
        
        glBindVertexArray(VAO); // vao can also kepp ebo
        glDrawArrays(GL_TRIANGLES, 0, 3); // use active shader program and binded VAO (with primitives data)
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // shader pr. and binded EBO
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
    }
    
    // de-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}
