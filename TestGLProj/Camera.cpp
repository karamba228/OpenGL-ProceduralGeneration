#include "Camera.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

//glm library
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include "iostream"

enum class Mode {
    DEFAULT,
    FIRST_PERSON,
    THIRD_PERSON
};

CameraMode current = CameraMode::THIRD_PERSON;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)),
    movementSpeed(SPEED),
    position(position),
    worldUp(up),
    yaw(yaw),
    pitch(pitch)
{
    updateCameraVectors();
}

CameraMode Camera::getMode() const {
    return current;
}

void Camera::setMode(CameraMode mode) {

    switch (mode) {
    case CameraMode::DEFAULT:
        std::cout << "Setting camera mode to DEFAULT\n";
        current = CameraMode::DEFAULT;
        pitch = 0.0f;
        yaw = -90.0f;
        position = glm::vec3(0.0f, 2.0f, 20.0f);
        break;
    case CameraMode::FIRST_PERSON:
        std::cout << "Setting camera mode to FIRST_PERSON\n";
        pitch = 0.0f;
        yaw = -90.0f;
        current = CameraMode::FIRST_PERSON;
        break;
    case CameraMode::THIRD_PERSON:
        std::cout << "Setting camera mode to THIRD_PERSON\n";
        current = CameraMode::THIRD_PERSON;
        pitch = -25.0f;
        position = glm::vec3(0.0f, 20.0f, 40.0f);
        break;
    }

    // Update the view matrix to reflect the changes in camera properties
    glm::mat4 viewMatrix = getViewMatrix();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::updateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);

    // Also re-calculate the right and up vector
    this->right = glm::normalize(glm::cross(front, worldUp));
    this->up = glm::normalize(glm::cross(right, front));
}


void Camera::updateCameraPosition(float deltaTime) {
    if (mvFw)
        moveForward(deltaTime);
    if (mvBw)
        moveBackward(deltaTime);
    if (mvLt)
        moveLeft(deltaTime);
    if (mvRt)
        moveRight(deltaTime);
    if (mvUp)
        moveUp(deltaTime);
    if (mvDw)
        moveDown(deltaTime);
    
}

void Camera::moveForward(float deltaTime) {
    std::cout << "camera should move forward" << std::endl;
    moveDirection = front;
    position += moveDirection * movementSpeed * deltaTime;
}

void Camera::moveBackward(float deltaTime) {
    moveDirection = -front;
    position += moveDirection * movementSpeed * deltaTime;
}

void Camera::moveLeft(float deltaTime) {
    moveDirection = -right;
    position += moveDirection * movementSpeed * deltaTime;
}

void Camera::moveRight(float deltaTime) {
    moveDirection = right;
    position += moveDirection * movementSpeed * deltaTime;
}

void Camera::moveUp(float deltaTime) {
    moveDirection = up;
    position += moveDirection * movementSpeed * deltaTime;
}

void Camera::moveDown(float deltaTime) {
    moveDirection = -up;
    position += moveDirection * movementSpeed * deltaTime;
}

void Camera::stopMoving() {
    moveDirection = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Camera::rotateYaw(float angle) {
    yaw += angle;
    updateCameraVectors();
}

void Camera::rotatePitch(float angle) {
    pitch += angle;

    if (pitch > MAX_PITCH) {
        pitch = MAX_PITCH;
    }
    else if (pitch < MIN_PITCH) {
        pitch = MIN_PITCH;
    }

    updateCameraVectors();
}
