// #include "glad.c"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "Extensions/tiny_obj_loader.h"

#include <iostream>
#include <string>

#include "Camera/Camera.cpp"
#include "Camera/OrthoCamera.hpp"
#include "Camera/PerspectiveCamera.hpp"

#include "Lighting/Light.hpp"
#include "Lighting/DirectionLight.hpp"
#include "Lighting/PointLight.hpp"

#include "Models/Model3D.cpp"

using namespace glm;
using namespace gd;

// static glm::vec3 camPos = glm::vec3(0.f);
// static glm::vec3 camRot = glm::vec3(0.f);

// static float x_mod = 0.f;
// static float y_mod = 0.f;
// static float z_mod = 0.f;
// static float scale_mod = 0.1f;
// static float fov_mod = 60;
// static float scale_x_mod = 0;
// static float scale_y_mod = 0;
// static float scale_z_mod = 0;
// static float theta_hor = 0;
// static float theta_ver = 0;

// static glm::vec4 rgba_mod(1.0f, 0.72f, 0.77f, 1.0f);
// static unsigned int rgbaLoc;

// static glm::vec3 cameraPos;
// static glm::mat4 cameraPosMatrix = glm::mat4(1.f);
static float height = 600.f;
static float width = 600.f;

static float rotationIncrement = 5.f;

static PerspectiveCamera *frontCamera;
static OrthoCamera *topCamera;

static Model3D *mainModel;
static Model3D *lightModel;

static PointLight *pointLight;
static DirectionLight *directionLight;

static bool useFrontCamera = true;
static bool controlLight = false;
// static bool divideViewport = false;

// static OrthoCamera topCamera(vec3(0.f, 10.f, 0.f));

void updateLightPosition()
{
    // vec4 pos = toMat4(quat(glm::radians(lightModel->rotation))) * vec4(lightModel->position, 1.f);
    // lightModel->position = vec3(pos);

    // lightModel->position;

    // float r = sqrt(lightModel->position.x * lightModel->position.x + lightModel->position.y * lightModel->position.y + lightModel->position.z * lightModel->position.z);
    // float longitude = atan2(lightModel->position.y, lightModel->position.x);
    // float lat = acos(lightModel->position.z / r);

    // lightModel->position.x = r * sin(lat) * cos(longitude);
    // lightModel->position.y = r * sin(lat) * sin(longitude);
    // lightModel->position.z = r * cos(lat);
    
    // lightModel->position.x = 10.f * sin(glm::radians(lightModel->rotation.z)) * cos(glm::radians(lightModel->rotation.x));
    // lightModel->position.y = 10.f * sin(glm::radians(lightModel->rotation.z)) * sin(glm::radians(lightModel->rotation.x));
    // lightModel->position.z = 10.f * cos(glm::radians(lightModel->rotation.z)) * cos(glm::radians(lightModel->rotation.y));

    lightModel->rotation = glm::radians(lightModel->rotation);
    lightModel->position = toMat4(quat(lightModel->rotation)) * vec4(lightModel->position, 1.f);

    // lightModel->position.x = (cos(glm::radians(lightModel->rotation.y)) * sin(glm::radians(lightModel->rotation.x))) * 10.f;
    // lightModel->position.y = sin(glm::radians(lightModel->rotation.y)) * 10.f;
    // lightModel->position.z = (cos(glm::radians(lightModel->rotation.y)) * cos(glm::radians(lightModel->rotation.x))) * 10.f;

    // lightModel->rotation.x = -(cos(glm::radians(lightModel->rotation.y)) * sin(glm::radians(lightModel->rotation.x))) * 10.f;
    // lightModel->rotation.y = -sin(glm::radians(lightModel->rotation.y)) * 10.f;
    // lightModel->rotation.z = -(cos(glm::radians(lightModel->rotation.y)) * cos(glm::radians(lightModel->rotation.x))) * 10.f;

    // pointLight->position = lightModel->position;
}

static void Key_Callback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mod)
{
    if (key == GLFW_KEY_1 && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        useFrontCamera = true;
    }

    if (key == GLFW_KEY_2 && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        useFrontCamera = false;
    }

    if (key == GLFW_KEY_SPACE && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        controlLight = !controlLight;
        if (controlLight)
        {
            pointLight->lightColor = vec3(1.0f, 0.72f, 0.77f);
            pointLight->ambientColor = vec3(1.0f, 0.72f, 0.77f);
            // pointLight->lightColor = vec3(1.f);
            // pointLight->ambientColor = vec3(1.f);
            lightModel->color = vec3(1.0f, 0.72f, 0.77f);
            mainModel->color = vec3(1.f);
        }
        else
        {
            pointLight->lightColor = vec3(1.f);
            pointLight->ambientColor = vec3(1.f);
            lightModel->color = vec3(1.f);
            mainModel->color = vec3(1.f);
        }
    }

    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        if (controlLight)
        {
            lightModel->rotation.y -= rotationIncrement;

            updateLightPosition();
            pointLight->position = lightModel->position;
            // frontCamera->rotation.y += 0.1f;
            // frontCamera->position.y = -sin(glm::radians(frontCamera->rotation.y)) * 10.f;
        }
        else
        {
            mainModel->rotation.y -= rotationIncrement;
        }
    }

    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        if (controlLight)
        {
            lightModel->rotation.y += rotationIncrement;

            updateLightPosition();
            pointLight->position = lightModel->position;
            // frontCamera->rotation.y += 0.1f;
            // frontCamera->position.y = -sin(glm::radians(frontCamera->rotation.y)) * 10.f;
        }
        else
        {
            mainModel->rotation.y += rotationIncrement;
        }
        
    }

    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        if (controlLight)
        {
            lightModel->rotation.x -= rotationIncrement;

            updateLightPosition();
            pointLight->position = lightModel->position;
            // frontCamera->rotation.y += 0.1f;
            // frontCamera->position.y = -sin(glm::radians(frontCamera->rotation.y)) * 10.f;
        }
        else
        {
            mainModel->rotation.x -= rotationIncrement;
        }
    }

    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        if (controlLight)
        {
            lightModel->rotation.x += rotationIncrement;
            
            updateLightPosition();
            pointLight->position = lightModel->position;
            // frontCamera->rotation.y += 0.1f;
            // frontCamera->position.y = -sin(glm::radians(frontCamera->rotation.y)) * 10.f;
        }
        else
        {
            mainModel->rotation.x += rotationIncrement;
        }
    }

    if (key == GLFW_KEY_E && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        if (controlLight)
        {
            lightModel->rotation.z -= rotationIncrement;

            updateLightPosition();
            pointLight->position = lightModel->position;
            // frontCamera->rotation.y += 0.1f;
            // frontCamera->position.y = -sin(glm::radians(frontCamera->rotation.y)) * 10.f;
        }
        else
        {
            mainModel->rotation.z -= rotationIncrement;
        }
    }

    if (key == GLFW_KEY_Q && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        if (controlLight)
        {
            lightModel->rotation.z += rotationIncrement;
            
            updateLightPosition();
            pointLight->position = lightModel->position;
            // frontCamera->rotation.y += 0.1f;
            // frontCamera->position.y = -sin(glm::radians(frontCamera->rotation.y)) * 10.f;
        }
        else
        {
            mainModel->rotation.z += rotationIncrement;
        }
    }
    
    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS) && controlLight)
    {
        directionLight->ambientStr += 0.1f;
        directionLight->specStr += 0.1f;
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS) && controlLight)
    {
        directionLight->ambientStr -= 0.1f;
        directionLight->specStr -= 0.1f;
    }

    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS) && controlLight)
    {
        pointLight->ambientStr += 0.1f;
        pointLight->specStr += 0.1f;
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS) && controlLight)
    {
        pointLight->ambientStr -= 0.1f;
        pointLight->specStr -= 0.1f;
    }


    // if (key == GLFW_KEY_X && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     fov_mod += 1.f;
    // }

    // if (key == GLFW_KEY_Z && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     fov_mod -= 1.f;
    // }
}

static void Cursor_Position_Callback(GLFWwindow *window, double xpos, double ypos)
{
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
    {
        if (useFrontCamera)
        {
            // some dark magic ritual that works!
            frontCamera->rotation.x += 0.1f * float(width / 2 - xpos);
            frontCamera->rotation.y += 0.1f * float(height / 2 - ypos);
            frontCamera->position.x = -(cos(glm::radians(frontCamera->rotation.y)) * sin(glm::radians(frontCamera->rotation.x))) * 10.f;
            frontCamera->position.y = -sin(glm::radians(frontCamera->rotation.y)) * 10.f;
            frontCamera->position.z = -(cos(glm::radians(frontCamera->rotation.y)) * cos(glm::radians(frontCamera->rotation.x))) * 10.f;
        }
        else
        {
            // topCamera->rotation.x += 0.1f * float(width / 2 - xpos);
            // topCamera->rotation.y += 0.1f * float(height / 2 - ypos);
            // std::cout << "x: " << topCamera->rotation.x << std::endl;
            // std::cout << "y: " << topCamera->rotation.y << std::endl;
        }
    }
}

void Mouse_Button_Callback(GLFWwindow *window, int button, int action, int mods)
{
    // double x = 0;
    // double y = 0;

    // click to reset
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // x_mod = 0;
        // y_mod = 0;
        // z_mod = 0;
        // scale_mod = 0.1;
        // fov_mod = 60;
        // scale_x_mod = 0;
        // scale_y_mod = 0;
        // scale_z_mod = 0;
        // theta_hor = 0;
        // theta_ver = 0;
    }
}

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(height, width, "Marcus Leocario", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);
    glfwSetCursorPosCallback(window, Cursor_Position_Callback);
    glfwSetMouseButtonCallback(window, Mouse_Button_Callback);

    GLuint shaderProgram;

    mainModel = new Model3D("Models/source/VoxelLink.obj", "Models/texture/VoxelLink.png", vec3(1.f), vec3(0.f, -2.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(0.2f));
    lightModel = new Model3D("Models/source/among us.obj", "", vec3(1.f), vec3(0.f, 0.f, -10.f), vec3(0.f, 0.f, 0.f), vec3(0.01f));

    frontCamera = new PerspectiveCamera(60, height, width, vec3(0.f, 2.f, 20.f), vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f));
    topCamera = new OrthoCamera(vec3(0.f, 20.f, 0.f), vec3(0.f, -90.f, 0.f), vec3(0.f, 0.f, 0.f));

    directionLight = new DirectionLight("dirLight", vec3(4, 11, -3), 1.f, 2.f, 32, vec3(0.f, 0.f, 1.f), vec3(0.7f, 0.7f, 1.f));
    pointLight = new PointLight("pointLight", lightModel->position + vec3(0.f, 2.f, 0.f), 0.5f, 0.7f, 32, vec3(1.f, 1.f, 1.f), vec3(1.f, 1.f, 1.f));

    std::cout << "loaded cameras" << std::endl;

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char *v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char *f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (useFrontCamera)
            glfwSetCursorPos(window, height / 2.f, width / 2.f);

        /* Render here */
        glFlush();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        directionLight->applyUniforms(shaderProgram);
        directionLight->applyExtraUniforms(shaderProgram);

        pointLight->applyUniforms(shaderProgram);
        pointLight->applyExtraUniforms(shaderProgram);

        if (useFrontCamera)
        {
            unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(frontCamera->generateProjectionMatrix()));

            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(frontCamera->generateViewMatrix()));
        }
        else
        {
            unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(topCamera->generateProjectionMatrix()));

            unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(topCamera->generateViewMatrix()));
        }

        // unsigned int gammaLoc = glGetUniformLocation(shaderProgram, "gamma");
        // glUniform1f(gammaLoc, 0.7f);

        // Draw
        mainModel->draw(shaderProgram);
        lightModel->draw(shaderProgram);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
