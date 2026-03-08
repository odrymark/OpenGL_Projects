#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

class Camera
{
    public:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        float yaw;
        float pitch;
        float moveSpeed;
        float sensitivity;

    Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
       glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
       float yaw = YAW,
       float pitch = PITCH)
    : position(position),
      worldUp(up),
      yaw(yaw),
      pitch(pitch),
      front(glm::vec3(0.f, 0.f, -1.f)),
      moveSpeed(SPEED),
      sensitivity(SENSITIVITY)
    {
        updateVectors();
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyInput(Camera_Movement direction, float deltaTime, bool isFlight)
    {
        float velocity = moveSpeed * deltaTime;

        glm::vec3 moveFront;
        if(!isFlight)
            moveFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        else
            moveFront = front;

        if(direction == FORWARD)
            position += moveFront * velocity;
        if(direction == BACKWARD)
            position -= moveFront * velocity;
        if(direction == RIGHT)
            position += right * velocity;
        if(direction == LEFT)
            position -= right * velocity;
    }

    void processMouseInput(float xOffset, float yOffset)
    {
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        updateVectors();
    }

    private:
        void updateVectors()
        {
            glm::vec3 f;
            f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            f.y = sin(glm::radians(pitch));
            f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(f);
            right = glm::normalize(glm::cross(front, worldUp));
            up = glm::normalize(glm::cross(right, front));
        }
};

#endif