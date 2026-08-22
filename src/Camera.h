#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera{
public:
    Camera();

    glm::mat4 viewMatrix() const;

    void moveForward(float distance);
    void moveRight(float distance);
    void moveVertical(float distance);

    void rotate(float yawOffset, float pitchOffset);

private:
    glm::vec3 forwardDirection() const;
    glm::vec3 rightDirection() const;

    glm::vec3 m_position;
    glm::vec3 m_up;

    float m_yaw;
    float m_pitch;
};
