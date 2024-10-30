#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(const glm::vec3& position)
        : position(position), target(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f),
        fov(glm::radians(60.0f)), nearPlane(0.1f), farPlane(1000.0f) {}

    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(position, target, up);
    }

    glm::mat4 GetProjectionMatrix(float aspectRatio) const {
        return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
    }

    void SetPosition(const glm::vec3& newPosition) {
        position = newPosition;
    }
    void SetTarget(const glm::vec3& newTarget) {
        target = newTarget;
    }

    void SetUp(const glm::vec3& newUp) {
        up = newUp;
    }

    glm::vec3 GetPosition() const { return position; }
    glm::vec3 GetTarget() const { return target; }
    glm::vec3 GetUp() const { return up; }

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    float fov;        // Field of view
    float nearPlane;  // Near clipping plane
    float farPlane;   // Far clipping plane
};

#endif // CAMERA_H
