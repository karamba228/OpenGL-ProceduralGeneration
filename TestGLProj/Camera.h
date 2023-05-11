#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum CameraMode {
    DEFAULT,
    FIRST_PERSON,
    THIRD_PERSON
};

class Camera {
public:
    
    glm::vec3 position;
    Camera(glm::vec3 position, glm::vec3 up, float yaw = YAW, float pitch = PITCH);

    bool mvFw = false;
    bool mvBw = false;
    bool mvLt = false;
    bool mvRt = false;
    bool mvUp = false;
    bool mvDw = false;

    CameraMode getMode() const;
    void Camera::setMode(CameraMode mode);
    glm::mat4 getViewMatrix() const;
    void updateCameraPosition(float deltaTime);
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);
    void stopMoving();

    void rotateYaw(float angle);
    void rotatePitch(float angle);

private:
    // Default camera values
    static constexpr float YAW = 0.0f;
    static constexpr float PITCH = -10.0f;
    static constexpr float SPEED = 5.0f;
    static constexpr float MAX_PITCH = 45.0f;
    static constexpr float MIN_PITCH = -45.0f;
    static constexpr float PITCH_INCREMENT = 5.0f;
    static constexpr float YAW_INCREMENT = 5.0f;

    CameraMode mode;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::vec3 moveDirection;
    glm::vec3 totalMoveDirection;
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    void updateCameraVectors();
};

#endif