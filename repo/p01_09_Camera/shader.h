#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"

class Shader {
public:
    //Shader(const std::string filePath); // Cherno solution
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void Use();
    
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 mat) const;

public:
    GLuint Program;
};

#endif /* shader_h */
