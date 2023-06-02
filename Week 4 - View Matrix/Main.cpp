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

static float x_mod = 0;
static float y_mod = 0;
static float z_mod = 0;
static float scale_mod = 20;
static float fov_mod = 60;
// static float scale_x_mod = 0;
// static float scale_y_mod = 0;
// static float scale_z_mod = 0;
static float theta_mod1 = 0;
static float theta_mod2 = 0;
static float height = 600.f;
static float width = 600.f;

static glm::vec4 rgba_mod(1.0f, 0.72f, 0.77f, 1.0f);
static unsigned int rgbaLoc;

// static glm::vec3 cameraPos;
// static glm::mat4 cameraPosMatrix = glm::mat4(1.f);

static GLuint shaderProgram;
static GLuint VAO, VBO, EBO;
static std::vector<GLuint> mesh_indices;

static bool divideViewport = true;

static void Key_Callback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mod)
{
    if (key == GLFW_KEY_D && action == GLFW_REPEAT)
    {
        x_mod += 0.05f;
    }

    if (key == GLFW_KEY_A && action == GLFW_REPEAT)
    {
        x_mod -= 0.05f;
    }

    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
    {
        y_mod += 0.05f;
    }

    if (key == GLFW_KEY_S && action == GLFW_REPEAT)
    {
        y_mod -= 0.05f;
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
    {
        theta_mod1 += 10.f;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
    {
        theta_mod1 -= 10.f;
    }

    if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
    {
        theta_mod2 += 10.f;
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
    {
        theta_mod2 -= 10.f;
    }

    if (key == GLFW_KEY_E && action == GLFW_REPEAT)
    {
        scale_mod += 0.1f;
    }

    if (key == GLFW_KEY_Q && action == GLFW_REPEAT)
    {
        scale_mod -= 0.1f;
    }

    if (key == GLFW_KEY_X && action == GLFW_REPEAT)
    {
        fov_mod += 1.f;
    }

    if (key == GLFW_KEY_Z && action == GLFW_REPEAT)
    {
        fov_mod -= 1.f;
    }
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
        x_mod = 0;
        y_mod = 0;
        z_mod = 0;
        scale_mod = 20;
        fov_mod = 60;
        // scale_x_mod = 0;
        // scale_y_mod = 0;
        // scale_z_mod = 0;
        theta_mod1 = 0;
        theta_mod2 = 0;
    }
}

glm::mat4 generateViewMatrix(glm::vec3 cameraPos)
{
    glm::mat4 cameraPosMatrix = glm::mat4(1.f);

    cameraPosMatrix = glm::translate(
        glm::mat4(1.0f),
        cameraPos * -1.0f);

    glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.f, 0));
    glm::vec3 cameraCenter = glm::vec3(x_mod, 3.f, 0);

    glm::vec3 F = cameraCenter - cameraPos;
    F = glm::normalize(F);

    glm::vec3 R = glm::normalize(glm::cross(F, worldUp));
    glm::vec3 U = glm::normalize(glm::cross(R, F));

    glm::mat4 cameraOrientation = glm::mat4(1.f);

    cameraOrientation[0][0] = R.x;
    cameraOrientation[1][0] = R.y;
    cameraOrientation[2][0] = R.z;

    cameraOrientation[0][1] = U.x;
    cameraOrientation[1][1] = U.y;
    cameraOrientation[2][1] = U.z;

    cameraOrientation[0][2] = -F.x;
    cameraOrientation[1][2] = -F.y;
    cameraOrientation[2][2] = -F.z;

    return cameraOrientation * cameraPosMatrix;
}

void frontView()
{
    glViewport(0, height / 2, width / 2, height / 2);
    rgba_mod = glm::vec4(1.f, 0.f, 0.f, 1.f);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(
        GL_TRIANGLES,
        mesh_indices.size(),
        GL_UNSIGNED_INT,
        0);
    rgbaLoc = glGetUniformLocation(shaderProgram, "rgba");
    glUniform4fv(rgbaLoc, 1, glm::value_ptr(rgba_mod));

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(x_mod + 10.f, 0.f, 0.f))));
}

void sideView()
{
    glViewport(width / 2, height / 2, width / 2, height / 2);
    rgba_mod = glm::vec4(1.0f, 0.72f, 0.77f, 1.0f);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(
        GL_TRIANGLES,
        mesh_indices.size(),
        GL_UNSIGNED_INT,
        0);
    rgbaLoc = glGetUniformLocation(shaderProgram, "rgba");
    glUniform4fv(rgbaLoc, 1, glm::value_ptr(rgba_mod));
    
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(x_mod, 0.f, -10.f))));
}

void topView()
{
    glViewport(width / 3, 0, width / 2, height / 2);
    rgba_mod = glm::vec4(0.f, 0.f, 1.f, 1.f);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(
        GL_TRIANGLES,
        mesh_indices.size(),
        GL_UNSIGNED_INT,
        0);
    rgbaLoc = glGetUniformLocation(shaderProgram, "rgba");
    glUniform4fv(rgbaLoc, 1, glm::value_ptr(rgba_mod));
    
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(x_mod, 10.f, -10.f))));
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

    std::string path = "Models/bunny.obj";
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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // glm::mat4 projection = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1.f);
        glm::mat4 projection = glm::perspective(glm::radians(fov_mod), height / width, 0.1f, 100.f);

        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 identity_matrix4 = glm::mat4(1.f);
        glm::mat4 transformation_matrix = glm::translate(identity_matrix4, glm::vec3(0, y_mod, z_mod));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_mod, scale_mod, scale_mod));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta_mod1), glm::vec3(0, 1, 0));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta_mod2), glm::vec3(1, 0, 0));

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        unsigned int xLoc = glGetUniformLocation(shaderProgram, "x");
        glUniform1f(xLoc, x_mod);

        unsigned int yLoc = glGetUniformLocation(shaderProgram, "y");
        glUniform1f(yLoc, y_mod);

        // Draw

        if (divideViewport)
        {
            frontView();
            sideView();
            topView();
        }
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
