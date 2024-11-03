#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>
#include <../glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera(GLFWwindow* window)
        : window(window), position(0.0f, 0.0f, 3.0f), front(0.0f, 0.0f, -1.0f),
        up(0.0f, 1.0f, 0.0f), yaw(-90.0f), pitch(0.0f), fov(60.0),
        firstMouse(true), lastX(400), lastY(300), baseSpeed(5.0f),
        sprintMultiplier(2.0f), usePerspective(true) {
        glfwSetCursorPosCallback(window, MouseCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetWindowUserPointer(window, this);
    }

    void ProcessInput(float deltaTime) {
        float cameraSpeed = baseSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cameraSpeed *= sprintMultiplier;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            position += cameraSpeed * front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            position -= cameraSpeed * front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            position -= up * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            position += up * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            ToggleProjection(); 
        }
        else {
            canToggleProjection = true; 
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstMouse = true;
        }
    }

    void ProcessMouseMovement(double xpos, double ypos) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
            if (firstMouse) {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xOffset = xpos - lastX;
            float yOffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;

            const float sensitivity = 0.3f;
            xOffset *= sensitivity;
            yOffset *= sensitivity;

            yaw += xOffset;
            pitch += yOffset;

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            UpdateCameraVectors();
        }
    }

    void ProcessZoom(double yOffset) {
        fov -= 5.0f * (float)yOffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 120.0f)
            fov = 120.0f;
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    glm::vec3 GetPosition() const { return position; }

    glm::mat4 GetProjectionMatrix(float aspectRatio) {
        if (usePerspective) {
            return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
        }
        else {
            float orthoSize = 5.0f;
            return glm::ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, 0.1f, 100.0f);
        }
    }

    void ToggleProjection() {
        if (canToggleProjection) {
            usePerspective = !usePerspective;
            canToggleProjection = false;
        }
    }

private:
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        camera->ProcessMouseMovement(xpos, ypos);
    }

    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        camera->ProcessZoom(yoffset);
    }

    void UpdateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);
    }

    GLFWwindow* window;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    float fov;

    bool firstMouse;
    float lastX, lastY;
    float baseSpeed;
    float sprintMultiplier;
    bool usePerspective;
    bool canToggleProjection;
};

#endif CAMERA_H
