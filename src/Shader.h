#pragma once

#include <glm/mat4x4.hpp>

#include <string>

class Shader{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    ~Shader();

    void use() const;

    void setMat4(const std::string& name, const glm::mat4& value) const;

    void setBool(const std::string& name, bool value) const;

private:
    unsigned int m_id;
};
