#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{
private:
    // camera Attributes
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

public:
    Camera(const glm::vec3 position);
    Camera(const glm::vec3 position, const glm::vec3 direction);
	~Camera();

    glm::vec3 Position;
    glm::vec3 Front;

    glm::mat4 getViewMatrix() const;
    float getZoom() const;

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);
private:
    void calcCameraVectors();
};
