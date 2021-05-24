#include "shader.h"
//#include <fstream>
//#include <sstream>

//Shader::Shader(const std::string filePath) {
//    std::ifstream stream(filePath);
//    
//    enum class ShaderType {
//      NONE = -1, VERTEX = 0, FRAGMENT = 1
//    };
//    
//    std::string line;
//    std::stringstream ss[2];
//    ShaderType type = ShaderType::NONE;
//    while (getline(stream, line)) {
//        if (line.find("#shader") != std::string::npos) {
//            if (line.find("vertex") != std::string::npos) {
//                type = ShaderType::VERTEX;
//            }
//            else if (line.find("fragment") != std::string::npos) {
//                type = ShaderType::FRAGMENT;
//            }
//        }
//        else {
//            ss[(int)type] << line << '\n';
//        }
//    }
//}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    // 1 Get Shader Code from filepath
    std::string vertexCode;
    std::string fragmentCode;
    
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    // check exceptions
    // badbit - irrecoverable stream error
    // failbit ??
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf(); // return given stream buufer
        fShaderStream << fShaderFile.rdbuf(); // if no given - returns null pointer
        
        vShaderFile.close();
        fShaderFile.close();
        
        // cast stream to GLchar array
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FAILED_TO_READ_FILE" << std::endl;
    }
    
    const GLchar* vShaderCode = (const GLchar*) vertexCode.c_str(); // c_str ???
    const GLchar* fShaderCode = (const GLchar*) fragmentCode.c_str();
    
    // 2 Build shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Shader program
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // check errors
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM:LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(Program);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(Program, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(Program, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1i(glGetUniformLocation(this->Program, name.c_str()), value);
}
