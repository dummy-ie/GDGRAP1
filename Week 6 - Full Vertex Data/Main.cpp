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

static glm::vec3 camPos = glm::vec3(0.f);
static glm::vec3 camRot = glm::vec3(0.f);

static float x_mod = 0;
static float y_mod = 0;
static float z_mod = 0;
static float scale_mod = 1;
static float fov_mod = 60;
// static float scale_x_mod = 0;
// static float scale_y_mod = 0;
// static float scale_z_mod = 0;
static float theta_hor = 0;
static float theta_ver = 0;
static float height = 600.f;
static float width = 600.f;

static glm::vec4 rgba_mod(1.0f, 0.72f, 0.77f, 1.0f);
static unsigned int rgbaLoc;

// static glm::vec3 cameraPos;
// static glm::mat4 cameraPosMatrix = glm::mat4(1.f);

static GLuint shaderProgram;
static GLuint VAO, VBO, EBO, VBO_UV;
static std::vector<GLuint> mesh_indices;
static std::vector<GLfloat> fullVertexData;


static bool divideViewport = false;

static void Key_Callback(
    GLFWwindow *window,
    int key,
    int scancode,
    int action,
    int mod)
{
    if (key == GLFW_KEY_D && action == GLFW_REPEAT)
    {
        x_mod += 0.1f;
    }

    if (key == GLFW_KEY_A && action == GLFW_REPEAT)
    {
        x_mod -= 0.1f;
    }

    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
    {
        y_mod += 0.1f;
    }

    if (key == GLFW_KEY_S && action == GLFW_REPEAT)
    {
        y_mod -= 0.1f;
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
    {
        theta_hor += 10.f;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
    {
        theta_hor -= 10.f;
    }

    if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
    {
        theta_ver += 10.f;
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
    {
        theta_ver -= 10.f;
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
        theta_hor = 0;
        theta_ver = 0;
    }
}

glm::mat4 generateViewMatrix(glm::vec3 cameraPos)
{
    glm::mat4 cameraPosMatrix = glm::mat4(1.f);

    cameraPosMatrix = glm::translate(
        glm::mat4(1.0f),
        cameraPos * -1.0f);

    glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.f, 0));
    glm::vec3 cameraCenter = glm::vec3(x_mod, 3.f, z_mod);

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
    if (divideViewport)
        glViewport(0, height / 2, width / 2, height / 2);
    else
        glViewport(0, 0, width, height);

    rgba_mod = glm::vec4(1.f, 0.f, 0.f, 1.f);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glDrawElements(
    //     GL_TRIANGLES,
    //     mesh_indices.size(),
    //     GL_UNSIGNED_INT,
    //     0);
    glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 5);
    rgbaLoc = glGetUniformLocation(shaderProgram, "rgba");
    glUniform4fv(rgbaLoc, 1, glm::value_ptr(rgba_mod));

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(x_mod + 10.f, 0.f, z_mod))));

    // rgba_mod = glm::vec4(1.0f, 0.72f, 0.77f, 1.0f);
    // glUseProgram(shaderProgram);
    // glBindVertexArray(VAO);
    // glDrawElements(
    //     GL_TRIANGLES,
    //     mesh_indices.size(),
    //     GL_UNSIGNED_INT,
    //     0);
    // rgbaLoc = glGetUniformLocation(shaderProgram, "rgba");
    // glUniform4fv(rgbaLoc, 1, glm::value_ptr(rgba_mod));

    // viewLoc = glGetUniformLocation(shaderProgram, "view");
    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(x_mod, 0.f, z_mod - 10.f))));
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
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(x_mod, 0.f, z_mod - 10.f))));
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
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(x_mod, 10.f, z_mod - 10.f))));
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
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str());

    // std::vector<GLuint> mesh_indices;
    // for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    // {
    //     mesh_indices.push_back(
    //         shapes[0].mesh.indices[i].vertex_index);
    // }

    // std::vector<GLfloat> fullVertexData;
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];
        fullVertexData.push_back(attributes.vertices.at(vData.vertex_index * 3)); 
        fullVertexData.push_back(attributes.vertices.at(vData.vertex_index * 3 + 1)); 
        fullVertexData.push_back(attributes.vertices.at(vData.vertex_index * 3 + 2)); 

        fullVertexData.push_back(attributes.vertices.at(vData.normal_index * 3)); 
        fullVertexData.push_back(attributes.vertices.at(vData.normal_index * 3 + 1)); 
        fullVertexData.push_back(attributes.vertices.at(vData.normal_index * 3 + 2)); 

        fullVertexData.push_back(attributes.texcoords.at(vData.texcoord_index * 2)); 
        fullVertexData.push_back(attributes.texcoords.at(vData.texcoord_index * 2 + 1)); 
    }
    // for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
    //     tinyobj::index_t vData = shapes[0].mesh.indices[i];
    //     // X
    //     fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
    //     // Y
    //     fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);
    //     // Z
    //     fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);

    //     fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
    //     fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
    // }
    // UV array
    // GLfloat UV[]{
    //     0.f, 1.f,   
    //     0.f, 0.f,
    //     1.f, 1.f,
    //     1.f, 0.f,
    //     1.f, 1.f,
    //     1.f, 0.f,
    //     0.f, 1.f,
    //     0.f, 0.f};

    // texture mapping
    int img_width, img_height, colorChannels;
    stbi_set_flip_vertically_on_load(true); // flip da image
    unsigned char *tex_bytes = stbi_load("Models/ayaya.png", &img_width, &img_height, &colorChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
    // generate MIPMAPS! (arent they the little image duplicates for when it gets far away??)
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free loaded bytes
    stbi_image_free(tex_bytes);
    glEnable(GL_DEPTH_TEST);

    // initialize VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &VBO_UV);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        // attributes.vertices.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3, // X Y Z
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GLfloat),
        (void *)0);
    
    GLintptr normalsPtr = 2 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GL_FLOAT),
        (void*)normalsPtr
    );
    
    GLintptr uvPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GL_FLOAT),
        (void*)uvPtr
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(
    //     GL_ELEMENT_ARRAY_BUFFER,
    //     sizeof(GLuint) * mesh_indices.size(),
    //     mesh_indices.data(),
    //     GL_STATIC_DRAW);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])), &UV[0], GL_DYNAMIC_DRAW);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    // glEnableVertexAttribArray(2);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glm::mat4 projection = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1.f);
        glm::mat4 projection = glm::perspective(glm::radians(fov_mod), height / width, 0.1f, 100.f);

        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 identity_matrix4 = glm::mat4(1.f);
        glm::mat4 transformation_matrix = glm::translate(identity_matrix4, glm::vec3(0, y_mod, 0));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_mod, scale_mod, scale_mod));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta_hor), glm::vec3(0, 1, 0));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta_ver), glm::vec3(1, 0, 0));

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        // unsigned int xLoc = glGetUniformLocation(shaderProgram, "x");
        // glUniform1f(xLoc, x_mod);

        unsigned int yLoc = glGetUniformLocation(shaderProgram, "y");
        glUniform1f(yLoc, y_mod);

        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glUniform1i(tex0Address, 0);

        // Draw

        frontView();

        if (divideViewport)
        {
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
