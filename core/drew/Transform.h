#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ew/ewMath/ewMath.h>

class Transform {
public:
    glm::vec3 position; // Position in 3D space
    glm::vec3 rotation; // Rotation in degrees (XYZ)
    glm::vec3 scale;    // Scale in 3D space

    // Constructor with default values
    Transform(glm::vec3 p = glm::vec3(0.0f), glm::vec3 r = glm::vec3(0.0f), glm::vec3 s = glm::vec3(0.5f))
        : position(p), rotation(r), scale(s) {}

    // Function to get the model matrix
    glm::mat4 GetModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f); // Start with the identity matrix

        // Apply translation
        model = glm::translate(model, position);

        // Apply rotation (in XYZ order)
        model = glm::rotate(model, ew::Radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis rotation
        model = glm::rotate(model, ew::Radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis rotation
        model = glm::rotate(model, ew::Radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis rotation

        // Apply scaling
        model = glm::scale(model, scale);

        return model;
    }
};
#endif TRANSFORM_H
