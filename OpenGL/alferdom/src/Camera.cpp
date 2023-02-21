#include "Camera.h"

Camera::Camera(const glm::vec3 newPos): Position(newPos), Front(glm::vec3(0.0, 0.0, -1.0)), WorldUp(glm::vec3(0.0, 1.0, 0.0)), Pitch(PITCH), Yaw(YAW), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	calcCameraVectors();
}

Camera::Camera(const glm::vec3 newPos, const glm::vec3 newFront) : Position(newPos), Front(newFront), Up(glm::vec3(0.0, 1.0, 0.0))
{}

Camera::~Camera()
{}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

float Camera::getZoom() const
{
    return Zoom;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;

    if (Position.x > 50.0f)
        Position.x = 50.0f;
    if (Position.y > 20.0f)
        Position.y = 20.0f;
    if (Position.z > 50.0f)
        Position.z = 50.0f;

    if (Position.x < -50.0f)
        Position.x = -50.0f;
    if (Position.y < -20.0f)
        Position.y = -20.0f;
    if (Position.z < -20.0f)
        Position.z = -20.0f;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    else if (Pitch < -89.0f)
        Pitch = -89.0f;

    calcCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    else if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::calcCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}