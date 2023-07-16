// #include "glad.c"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "Extensions/tiny_obj_loader.h"

#include <iostream>
#include <string>

#include "Model3D.cpp"

static glm::vec3 camDir;
static glm::vec3 camPos = glm::vec3(0.f, 0.f, 0.f);
static float fov_mod = 60;
static float theta_hor = 0;
static float theta_ver = 0;

static glm::vec3 worldRight = glm::vec3(
    sin(theta_hor - 3.14f / 2.0f),
    0,
    cos(theta_hor - 3.14f / 2.0f));

static float height = 600.f;
static float width = 600.f;

static GLuint shaderProgram;
static GLuint VAO, VBO, EBO;
static std::vector<GLuint> mesh_indices;

static std::vector<model::Model3D> objects;

static bool divideViewport = false;

glm::mat4 generateViewMatrix(glm::vec3 cameraPos)
{
    // calculate the camera's facing direction
    camDir = glm::vec3(
        cos(glm::radians(theta_ver)) * sin(glm::radians(theta_hor)),
        sin(glm::radians(theta_ver)),
        cos(glm::radians(theta_ver)) * cos(glm::radians(theta_hor)));

    // Recalculate the right vector relative to camera view
    worldRight = glm::vec3(
        sin(glm::radians(theta_hor) - 3.14f / 2.0f),
        0,
        cos(glm::radians(theta_hor) - 3.14f / 2.0f));

    // world Up : perpendicular to look direction and world right
    glm::vec3 worldUp = glm::cross(worldRight, camDir);
    // glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.f, 0));
    glm::vec3 cameraCenter = cameraPos + camDir; // camera position combined with its face direction

    // GLFW function for generating a view matrix with our variables
    return glm::lookAt(cameraPos, cameraCenter, worldUp);
}

static void Key_Callback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mod)
{
    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        camPos += worldRight * 0.05f; // strafe right
    }

    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        camPos -= worldRight * 0.05f; // strafe left
    }

    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        camPos += camDir * 0.05f; // move forward
    }

    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        camPos -= camDir * 0.05f; // move backward
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        theta_hor -= 0.05f; // look right
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        theta_hor += 0.05f; // look left
    }

    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        theta_ver += 0.05f; // look up
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        theta_ver -= 0.05f; // look down
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        if (glfwGetTime() > 3) // if the time has exceeded 3 seconds, the cooldown is over.
        {
            // spawn an object in front of the camera a little lower to account for the object length, and scaled down by half.
            objects.push_back(Model3D((camPos + camDir) + glm::vec3(0, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.5f)));
            glfwSetTime(0); // reset timer
        }
    }
}

static void Cursor_Position_Callback(GLFWwindow *window, double xpos, double ypos)
{
    // if the window is focused, rotate the camera by the mouse movement in relation to the window size.
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
    {
        theta_hor += 0.1f * float(width / 2 - xpos);
        theta_ver += 0.1f * float(height / 2 - ypos);
    }
}

void Mouse_Button_Callback(GLFWwindow *window, int button, int action, int mods)
{
    // click to reset the camera position and fov
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        camPos.x = 0;
        camPos.y = 0;
        camPos.z = 0;
        fov_mod = 60;
        theta_hor = 0;
        theta_ver = 0;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // scroll forwards to zoom in, and vice versa.
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
        fov_mod -= yoffset * 2;
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
    glfwSetScrollCallback(window, scroll_callback);

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

    // Master Sword from https://free3d.com/3d-model/master-sword-7626.html
    std::string path = "Models/mastersword.obj";
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str());

    // std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index);
    }

    // initialize VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        // attributes.vertices.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3, // X Y Z
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GLfloat),
        (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW);

    glfwSwapInterval(1); // vsync
    glEnable(GL_CULL_FACE); // cull faces behind other objects

    objects.push_back(Model3D());

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSetCursorPos(window, height / 2.f, width / 2.f);

        // glm::mat4 projection = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1.f);
        glm::mat4 projection = glm::ortho(-(width / 10.f), width / 10.f, -(height / 10.f), height / 10.f, 0.1f, 100.f);
        // glm::mat4 projection = glm::perspective(glm::radians(fov_mod), height / width, 0.1f, 100.f); // perspective projection

        // Projection Matrix
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // View Matrix
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(camPos)));

        // Draw
        for (auto &object : objects) // for every object we have, render it using its stored transform.
            object.draw(shaderProgram, VAO, mesh_indices);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
