#pragma once

#include <string>

class Shader{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    ~Shader();

    void use() const;

private:
    unsigned int m_id;
};
