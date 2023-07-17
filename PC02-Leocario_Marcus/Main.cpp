// #include "glad.c"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

static bool useFrontCamera = true;
// static bool divideViewport = false;

// static OrthoCamera topCamera(vec3(0.f, 10.f, 0.f));

static void Key_Callback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mod)
{
    // if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     x_mod += 0.1f;
    // }

    // if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     x_mod -= 0.1f;
    // }

    // if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     z_mod += 0.1f;
    // }

    // if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     z_mod -= 0.1f;
    // }

    // if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     theta_hor += 10.f;
    // }

    // if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     theta_hor -= 10.f;
    // }

    // if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     theta_ver += 10.f;
    // }

    // if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     theta_ver -= 10.f;
    // }

    // if (key == GLFW_KEY_E && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     scale_mod += 0.1f;
    // }

    // if (key == GLFW_KEY_Q && (action == GLFW_REPEAT || action == GLFW_PRESS))
    // {
    //     scale_mod -= 0.1f;
    // }

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

void frontView()
{
    // if (divideViewport)
    //     glViewport(0, height / 2, width / 2, height / 2);
    // else
    //     glViewport(0, 0, width, height);

    // rgba_mod = glm::vec4(1.f, 0.f, 0.f, 1.f);
}

void topView()
{
    // glViewport(width / 3, 0, width / 2, height / 2);
    // rgba_mod = glm::vec4(0.f, 0.f, 1.f, 1.f);
    // glUseProgram(shaderProgram);
    // glBindVertexArray(VAO);
    // glDrawElements(
    //     GL_TRIANGLES,
    //     mesh_indices.size(),
    //     GL_UNSIGNED_INT,
    //     0);
    // rgbaLoc = glGetUniformLocation(shaderProgram, "rgba");
    // glUniform4fv(rgbaLoc, 1, glm::value_ptr(rgba_mod));

    // unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(x_mod, 10.f, z_mod - 10.f))));
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

    Model3D mainModel("Models/source/VoxelLink.obj", "Models/texture/VoxelLink.png", vec3(0.f, 0.f, 0.f), vec3(0.f, 180.f, 0.f), vec3(0.2f));
    Model3D lightModel("Models/source/among us.obj", "Models/texture/Plastic_4K_Diffuse.jpg", vec3(0.f, -5.f, 0.f), vec3(0.f, 180.f, 0.f), vec3(0.01f));

    PerspectiveCamera frontCamera(60, height, width, vec3(0.f, 0.f, -10.f));
    OrthoCamera topCamera;

    DirectionLight directionLight("dirLight", vec3(4, 11, -3), 0.5f, 0.7f, 16, vec3(1.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));
    PointLight pointLight("pointLight", vec3(0.f), 0.7f, 0.9f, 16, vec3(0.f, 1.f, 0.f), vec3(1.f, 1.f, 1.f));

    std::cout << "loaded camera" << std::endl;

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

    // glm::vec3 lightPos = glm::vec3(0.f, 3.f, 0.f);
    // glm::vec3 lightColor = glm::vec3(1.0f, 0.72f, 0.77f);
    // float ambientStr = 0.5f;
    // glm::vec3 ambientColor = glm::vec3(1.f, 1.f, 1.f);
    // float specStr = 0.7f;
    // float specPhong = 16;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glFlush();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glm::mat4 projection = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1.f);
        // glm::ortho();
        // glm::mat4 projection = glm::perspective(glm::radians(fov_mod), height / width, 0.1f, 100.f);

        glUseProgram(shaderProgram);

        directionLight.applyUniforms(shaderProgram);
        directionLight.applyExtraUniforms(shaderProgram);
        
        pointLight.applyUniforms(shaderProgram);
        pointLight.applyExtraUniforms(shaderProgram);

        // unsigned int ambientStrLoc = glGetUniformLocation(shaderProgram, "pointLight.ambientStr");
        // glUniform1f(ambientStrLoc, pointLight.ambientStr);

        // unsigned int specStrLoc = glGetUniformLocation(shaderProgram, "pointLight.specStr");
        // glUniform1f(specStrLoc, pointLight.specStr);
        
        // unsigned int specPhongLoc = glGetUniformLocation(shaderProgram, "pointLight.specPhong");
        // glUniform1f(specPhongLoc, pointLight.specPhong);

        // unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "pointLight.lightColor");
        // glUniform3fv(lightColorLoc, 1, value_ptr(pointLight.lightColor));

        // unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, "pointLight.ambientColor");
        // glUniform3fv(ambientColorLoc, 1, value_ptr(pointLight.ambientColor));

        // unsigned int positionLoc = glGetUniformLocation(shaderProgram, "pointLight.position");
        // glUniform3fv(positionLoc, 1, value_ptr(pointLight.position));

        // unsigned int constantLoc = glGetUniformLocation(shaderProgram, "pointLight.constant");
        // glUniform1f(constantLoc, pointLight.constant);

        // unsigned int linearLoc = glGetUniformLocation(shaderProgram, "pointLight.linear");
        // glUniform1f(linearLoc, pointLight.linear);
        
        // unsigned int quadraticLoc = glGetUniformLocation(shaderProgram, "pointLight.quadratic");
        // glUniform1f(quadraticLoc, pointLight.quadratic);

        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(frontCamera.generateProjectionMatrix()));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(frontCamera.generateViewMatrix()));

        // std::cout << "trying to draw" << std::endl;
        mainModel.draw(shaderProgram);
        lightModel.draw(shaderProgram);
        // std::cout << "end draw" << std::endl;


        // Draw

        // if (useFrontCamera)
        //     frontView();
        // else
        //     topView();

        // if (divideViewport)
        // {
        //     sideView();
        // }

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
