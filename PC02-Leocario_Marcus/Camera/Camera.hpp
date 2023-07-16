#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gd
{
    using namespace glm;
    class Camera
    {
    protected:
        vec3 position = vec3(0.f, 0.f, -10.f);
        vec3 rotation = vec3(0.f);
        vec3 direction = vec3(0.f);
        vec3 cameraRight;

    public:
        Camera(vec3 pos = vec3(0.f, 0.f, -10.f), vec3 rot = vec3(0.f), vec3 dir = vec3(0.f));
        mat4 generateViewMatrix();
        virtual mat4 generateProjectionMatrix() = 0;
    };
} // namespace gd

#endif // !CAMERA_HPP