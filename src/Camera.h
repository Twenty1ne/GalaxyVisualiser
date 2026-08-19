#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera{
public:
    Camera();

    glm::mat4 viewMatrix() const;

private:
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_up;
};
