#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

Camera::Camera() :
    m_position(0.0f, 0.0f, 3.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_yaw(-90.0f),
    m_pitch(0.0f){}

glm::vec3 Camera::forwardDirection() const{
    glm::vec3 direction;

    direction.x =
        glm::cos(glm::radians(m_yaw)) *
        glm::cos(glm::radians(m_pitch));
    direction.y =
        glm::sin(glm::radians(m_pitch));
    direction.z =
        glm::sin(glm::radians(m_yaw)) *
        glm::cos(glm::radians(m_pitch));

    return glm::normalize(direction);

}

glm::vec3 Camera::rightDirection() const{
    glm::vec3 forward = forwardDirection();

    forward.y = 0.0f;

    forward = glm::normalize(forward);

    return glm::normalize(glm::cross(forward, m_up));
}

glm::mat4 Camera::viewMatrix() const{
    glm::vec3 forward = forwardDirection();

    return glm::lookAt(
        m_position,
        m_position + forward,
        m_up
    );
}

void Camera::moveForward(float distance){
    m_position += forwardDirection() * distance;
}

void Camera::moveRight(float distance){
    m_position += rightDirection() * distance;
}

void Camera::moveVertical(float distance){
    m_position += m_up * distance;
}

void Camera::rotate(float yawOffset, float pitchOffset){
    m_yaw += yawOffset;
    m_pitch += pitchOffset;

    if(m_pitch > 89.0f){
        m_pitch = 89.0f;
    }

    if(m_pitch < -89.0f){
        m_pitch = -89.0f;
    }
}
