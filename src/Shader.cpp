#include "Shader.h"

#include <glad/gl.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath){
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);

    if(!vertexFile || !fragmentFile){
        std::cerr << "Failed to open shader files\n";
        m_id = 0;
        return;
    }

    std::stringstream vertexStream;
    std::stringstream fragmentStream;

    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    const std::string vertexSource = vertexStream.str();
    const std::string fragmentSource = fragmentStream.str();

    const char* vertexCode = vertexSource.c_str();
    const char* fragmentCode = fragmentSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n" << infoLog << '\n';
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed:\n" << infoLog << '\n';
    }

    m_id = glCreateProgram();

    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    if(!success){
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed:\n" << infoLog << '\n';
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader(){
    if(m_id != 0){
        glDeleteProgram(m_id);
    }
}

void Shader::use() const {
    glUseProgram(m_id);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const{
    int location = glGetUniformLocation(m_id, name.c_str());

    glUniformMatrix4fv(
        location,
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value ? 1 : 0);
}
