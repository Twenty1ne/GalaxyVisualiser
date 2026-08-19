#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>

Camera::Camera() :
    m_position(0.0f, 0.0f, 3.0f),
    m_target(0.0f, 0.0f, 0.0f),
    m_up(0.0f, 1.0f, 0.0f){}

glm::mat4 Camera::viewMatrix() const{
    return glm::lookAt(
        m_position,
        m_target,
        m_up
    );
}
