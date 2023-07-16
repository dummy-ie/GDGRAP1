#include "Camera.hpp"

using namespace gd;

Camera::Camera(vec3 pos, vec3 rot, vec3 dir) : position(pos), rotation(rot), direction(dir)
{
    cameraRight = glm::vec3(
            sin(rotation.x - 3.14f / 2.0f),
            0,
            cos(rotation.x - 3.14f / 2.0f));
}

mat4 Camera::generateViewMatrix()
{
    // calculate the camera's facing direction
    direction = glm::vec3(
        cos(rotation.y) * sin(rotation.x),
        sin(rotation.y),
        cos(rotation.y) * cos(rotation.x));

    // Recalculate the right vector relative to camera view
    cameraRight = glm::vec3(
        sin(rotation.x - 3.14f / 2.0f),
        0,
        cos(rotation.x - 3.14f / 2.0f));

    // world Up : perpendicular to look direction and world right
    glm::vec3 cameraUp = glm::cross(cameraRight, direction);
    // glm::vec3 cameraUp = glm::normalize(glm::vec3(0, 1.f, 0));
    glm::vec3 cameraCenter = position + direction; // camera position combined with its face direction

    // GLFW function for generating a view matrix with our variables
    return glm::lookAt(position, cameraCenter, cameraUp);
}
