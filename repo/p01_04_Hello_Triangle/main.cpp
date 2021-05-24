// www.docs.gl

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// Shaders
const GLchar* vertexShaderSource = "#version 330 core \n"
"layout (location = 0) in vec3 position;\n"
"void main() {\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\n\0";

const GLchar* fragmentShaderSource = "#version 330 core \n"
"out vec4 color;\n"
"void main() {\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
    
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // bind vertexShader with it's code
    glCompileShader(vertexShader); // compile shader
    // test
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // check if compiled
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // check if compiled
    // test
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Shader program
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // test
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader); // Delete shaders after attaching
    glDeleteShader(fragmentShader);
    
    
    // triangle
    GLfloat verticies[] =  {
         0.5f,  0.5f, 0.0f, // top right corner
         0.5f, -0.5f, 0.0f, // bot right
        -0.5f, -0.5f, 0.0f, // bot left
        -0.5f,  0.5f, 0.0f  // top left
    };
    GLuint indices[] = {
        0, 1, 3, // 1st triangle
        1, 2, 3 //  2nd one
    };
    GLuint VBO; // Vertex Buffer Object
    GLuint VAO; // Vertex Array Object
    GLuint EBO; // Element Buffer Object - for index drawing
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // bind VAO
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind gl_array_buffer with vbo
    // copy vbo data to gl_array_buffer
    // https://docs.gl/gl3/glBufferData
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
    // define an array of generic vertex attribute data
    // https://docs.gl/gl3/glVertexAttribPointer
    // GLvoid* is kinda offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0 );
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0); // unbind VAO
    
    // wireframe polygons
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Game loop
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // get events (keys/mouse etc.)
        
        glClearColor(0.2f, 0.3f, 0.3f, 10.f);
        glClear(GL_COLOR_BUFFER_BIT); // fix for red flickering
        
        glUseProgram(shaderProgram); // use shader program
        glBindVertexArray(VAO); // vao can also kepp ebo
        // glDrawArrays(GL_TRIANGLES, 0, 3); // use active shader program and binded VAO (with primitives data)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // shader pr. and binded EBO
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
    }
    
    // de-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}
