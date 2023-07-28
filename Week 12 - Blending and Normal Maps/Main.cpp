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

using namespace glm;

static glm::vec3 camPos = glm::vec3(0.f, 0.f, -10.f);
static glm::vec3 camRot = glm::vec3(0.f);
static glm::vec3 camDir;
static float fov_mod = 60;
static float cam_hor = 0;
static float cam_ver = 0;

static glm::vec3 worldRight = glm::vec3(
    sin(cam_hor - 3.14f / 2.0f),
    0,
    cos(cam_hor - 3.14f / 2.0f));

static float x_mod = 0.f;
static float y_mod = 0.f;
static float z_mod = -10.f;
static float scale_mod = 1.f;
// static float fov_mod = 60;
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
    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        x_mod += 0.1f;
    }

    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        x_mod -= 0.1f;
    }

    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        z_mod += 0.1f;
    }

    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        z_mod -= 0.1f;
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        theta_hor += 10.f;
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        theta_hor -= 10.f;
    }

    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        theta_ver += 10.f;
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        theta_ver -= 10.f;
    }

    if (key == GLFW_KEY_E && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        scale_mod += 0.1f;
    }

    if (key == GLFW_KEY_Q && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        scale_mod -= 0.1f;
    }

    if (key == GLFW_KEY_X && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        fov_mod += 1.f;
    }

    if (key == GLFW_KEY_Z && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        fov_mod -= 1.f;
    }
}

static void Cursor_Position_Callback(GLFWwindow *window, double xpos, double ypos)
{
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
    {
        cam_hor += 0.1f * float(width / 2 - xpos);
        cam_ver += 0.1f * float(height / 2 - ypos);
    }
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
        scale_mod = 0.1;
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
    // calculate the camera's facing direction
    camDir = glm::vec3(
        cos(glm::radians(cam_ver)) * sin(glm::radians(cam_hor)),
        sin(glm::radians(cam_ver)),
        cos(glm::radians(cam_ver)) * cos(glm::radians(cam_hor)));

    // Recalculate the right vector relative to camera view
    worldRight = glm::vec3(
        sin(glm::radians(cam_hor) - 3.14f / 2.0f),
        0,
        cos(glm::radians(cam_hor) - 3.14f / 2.0f));

    // world Up : perpendicular to look direction and world right
    glm::vec3 worldUp = glm::cross(worldRight, camDir);
    // glm::vec3 worldUp = glm::normalize(glm::vec3(0, 1.f, 0));
    glm::vec3 cameraCenter = cameraPos + camDir; // camera position combined with its face direction

    // GLFW function for generating a view matrix with our variables
    return glm::lookAt(cameraPos, cameraCenter, worldUp);
}

// void frontView()
// {
//     if (divideViewport)
//         glViewport(0, height / 2, width / 2, height / 2);
//     else
//         glViewport(0, 0, width, height);

//     rgba_mod = glm::vec4(1.f, 0.f, 0.f, 1.f);
//     glUseProgram(shaderProgram);
//     glBindVertexArray(VAO);
//     // glDrawElements(
//     //     GL_TRIANGLES,
//     //     mesh_indices.size(),
//     //     GL_UNSIGNED_INT,
//     //     0);
//     glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
//     rgbaLoc = glGetUniformLocation(shaderProgram, "rgba");
//     glUniform4fv(rgbaLoc, 1, glm::value_ptr(rgba_mod));

//     unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
//     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(0.f, 0.f, -10.f))));
// }

// void sideView()
// {
//     glViewport(width / 2, height / 2, width / 2, height / 2);
//     rgba_mod = glm::vec4(1.0f, 0.72f, 0.77f, 1.0f);
//     glUseProgram(shaderProgram);
//     glBindVertexArray(VAO);
//     glDrawElements(
//         GL_TRIANGLES,
//         mesh_indices.size(),
//         GL_UNSIGNED_INT,
//         0);
//     rgbaLoc = glGetUniformLocation(shaderProgram, "rgba");
//     glUniform4fv(rgbaLoc, 1, glm::value_ptr(rgba_mod));

//     unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
//     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(x_mod, 0.f, z_mod - 10.f))));
// }

// void topView()
// {
//     glViewport(width / 3, 0, width / 2, height / 2);
//     rgba_mod = glm::vec4(0.f, 0.f, 1.f, 1.f);
//     glUseProgram(shaderProgram);
//     glBindVertexArray(VAO);
//     glDrawElements(
//         GL_TRIANGLES,
//         mesh_indices.size(),
//         GL_UNSIGNED_INT,
//         0);
//     rgbaLoc = glGetUniformLocation(shaderProgram, "rgba");
//     glUniform4fv(rgbaLoc, 1, glm::value_ptr(rgba_mod));

//     unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
//     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(generateViewMatrix(glm::vec3(x_mod, 10.f, z_mod - 10.f))));
// }

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

    std::fstream sky_vertSrc("Shaders/skybox.vert");
    std::stringstream sky_vertBuff;
    sky_vertBuff << sky_vertSrc.rdbuf();
    std::string sky_vertS = sky_vertBuff.str();
    const char *sky_v = sky_vertS.c_str();

    std::fstream sky_fragSrc("Shaders/skybox.frag");
    std::stringstream sky_fragBuff;
    sky_fragBuff << sky_fragSrc.rdbuf();
    std::string sky_fragS = sky_fragBuff.str();
    const char *sky_f = sky_fragS.c_str();

    GLuint sky_vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sky_vertShader, 1, &sky_v, NULL);
    glCompileShader(sky_vertShader);

    GLuint sky_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sky_fragShader, 1, &sky_f, NULL);
    glCompileShader(sky_fragShader);

    GLuint skyboxProgram = glCreateProgram();
    glAttachShader(skyboxProgram, sky_vertShader);
    glAttachShader(skyboxProgram, sky_fragShader);

    glLinkProgram(skyboxProgram);

    // glDeleteShader(sky_vertShader);
    // glDeleteShader(sky_fragShader);

    /*
  7--------6
 /|       /|
4--------5 |
| |      | |
| 3------|-2
|/       |/
0--------1
*/
    // Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f,  // 0
        1.f, -1.f, 1.f,   // 1
        1.f, -1.f, -1.f,  // 2
        -1.f, -1.f, -1.f, // 3
        -1.f, 1.f, 1.f,   // 4
        1.f, 1.f, 1.f,    // 5
        1.f, 1.f, -1.f,   // 6
        -1.f, 1.f, -1.f   // 7
    };

    // Skybox Indices
    unsigned int skyboxIndices[]{
        1, 2, 6,
        6, 5, 1,

        0, 4, 7,
        7, 3, 0,

        4, 5, 6,
        6, 7, 4,

        0, 3, 2,
        2, 1, 0,

        0, 1, 5,
        5, 4, 0,

        3, 7, 6,
        6, 2, 3};

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    std::string facesSkybox[]{
        "Models/Skybox/rainbow_rt.png",
        "Models/Skybox/rainbow_lf.png",
        "Models/Skybox/rainbow_up.png",
        "Models/Skybox/rainbow_dn.png",
        "Models/Skybox/rainbow_ft.png",
        "Models/Skybox/rainbow_bk.png",
    };

    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);

        unsigned char *data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        stbi_image_free(data);
    }

    stbi_set_flip_vertically_on_load(true);

    std::string path = "Models/djSword.obj";
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

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i+=3)
    {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[vData1.vertex_index * 3 + 1],
            attributes.vertices[vData1.vertex_index * 3 + 2]
        );
        
        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[vData2.vertex_index * 3 + 1],
            attributes.vertices[vData2.vertex_index * 3 + 2]
        );

        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[vData3.vertex_index * 3 + 1],
            attributes.vertices[vData3.vertex_index * 3 + 2]
        );

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[vData1.texcoord_index * 2],
            attributes.texcoords[vData1.texcoord_index * 2 + 1]
        );

        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[vData2.texcoord_index * 2],
            attributes.texcoords[vData2.texcoord_index * 2 + 1]
        );

        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[vData3.texcoord_index * 2],
            attributes.texcoords[vData3.texcoord_index * 2 + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
        
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

    // std::vector<GLfloat> fullVertexData;
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];
        fullVertexData.push_back(attributes.vertices.at((vData.vertex_index * 3)));
        fullVertexData.push_back(attributes.vertices.at((vData.vertex_index * 3) + 1));
        fullVertexData.push_back(attributes.vertices.at((vData.vertex_index * 3) + 2));

        fullVertexData.push_back(attributes.normals.at((vData.normal_index * 3)));
        fullVertexData.push_back(attributes.normals.at((vData.normal_index * 3) + 1));
        fullVertexData.push_back(attributes.normals.at((vData.normal_index * 3) + 2));

        fullVertexData.push_back(attributes.texcoords.at((vData.texcoord_index * 2)));
        fullVertexData.push_back(attributes.texcoords.at((vData.texcoord_index * 2) + 1));

        fullVertexData.push_back(tangents.at(i).x);
        fullVertexData.push_back(tangents[i].y);
        fullVertexData.push_back(tangents[i].z);
        fullVertexData.push_back(bitangents[i].x);
        fullVertexData.push_back(bitangents[i].y);
        fullVertexData.push_back(bitangents[i].z);
    }

    // texture mapping
    int img_width, img_height, colorChannels;
    stbi_set_flip_vertically_on_load(true); // flip da image
    unsigned char *tex_bytes = stbi_load("Models/13Mats/brickwall.jpg", &img_width, &img_height, &colorChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
    // generate MIPMAPS! (arent they the little image duplicates for when it gets far away??)
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free loaded bytes
    stbi_image_free(tex_bytes);
    glEnable(GL_DEPTH_TEST);
    
    int img_width2, img_height2, colorChannels2;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *normal_bytes = stbi_load("Models/13Mats/brickwall_normal.jpg", &img_width2, &img_height2, &colorChannels2, 0);

    GLuint norm_tex;
    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width2, img_height2, 0, GL_RGB, GL_UNSIGNED_BYTE, normal_bytes);
    // generate MIPMAPS! (arent they the little image duplicates for when it gets far away??)
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free loaded bytes
    stbi_image_free(normal_bytes);
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
        GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        0,
        3, // X Y Z
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void *)0);

    GLintptr normalsPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void *)normalsPtr);

    glm::vec3 lightPos = glm::vec3(0.f, 0.f, -10.f);
    glm::vec3 lightColor = glm::vec3(1.0f, 0.72f, 0.77f);
    float ambientStr = 1.0f;
    glm::vec3 ambientColor = glm::vec3(1.f, 1.f, 1.f);
    float specStr = 2.0f;
    float specPhong = 32;

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void *)uvPtr);

    GLintptr tangentPtr = 8 * sizeof(float);
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void *)tangentPtr);
    
    GLintptr bitangentPtr = 11 * sizeof(float);
    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void *)bitangentPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwSetCursorPos(window, height / 2.f, width / 2.f);

        /* Render here */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glBlendEquation(GL_FUNC_SUBTRACT);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camPos = vec3(x_mod, y_mod, z_mod);

        glm::mat4 projection = glm::perspective(glm::radians(fov_mod), height / width, 0.1f, 100.f);

        glm::mat4 viewMatrix = generateViewMatrix(camPos);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glUseProgram(skyboxProgram);

        glm::mat4 skyView = glm::mat4(1.f);
        skyView = glm::mat4(
            glm::mat3(viewMatrix));

        unsigned int sky_ViewLoc = glGetUniformLocation(skyboxProgram, "view");
        glUniformMatrix4fv(sky_ViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));

        unsigned int sky_ProjectionLoc = glGetUniformLocation(skyboxProgram, "projection");
        glUniformMatrix4fv(sky_ProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glUseProgram(shaderProgram);

        // glm::mat4 projection = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1.f);

        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 identity_matrix4 = glm::mat4(1.f);
        glm::mat4 transformation_matrix = glm::translate(identity_matrix4, glm::vec3(0.f, 0.f, 0.f));
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_mod, scale_mod, scale_mod));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta_hor), glm::vec3(0, 1, 0));
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta_ver), glm::vec3(1, 0, 0));
        // transformation_matrix = glm::rotate(transformation_matrix, glm::radians(90), glm::vec3(0, 0, 1));

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        // unsigned int xLoc = glGetUniformLocation(shaderProgram, "x");
        // glUniform1f(xLoc, x_mod);

        unsigned int yLoc = glGetUniformLocation(shaderProgram, "y");
        glUniform1f(yLoc, y_mod);

        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glUniform1i(tex0Address, 0);
        
        glActiveTexture(GL_TEXTURE1);
        GLuint normtexAddress = glGetUniformLocation(shaderProgram, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(normtexAddress, 1);

        GLuint lightAddress = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint cameraPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(camPos));

        GLuint specStrAddress = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        // Draw

        rgba_mod = glm::vec4(1.f, 0.f, 0.f, 1.f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawElements(
        //     GL_TRIANGLES,
        //     mesh_indices.size(),
        //     GL_UNSIGNED_INT,
        //     0);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
        rgbaLoc = glGetUniformLocation(shaderProgram, "rgba");
        glUniform4fv(rgbaLoc, 1, glm::value_ptr(rgba_mod));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        // frontView();

        // if (divideViewport)
        // {
        //     sideView();
        //     topView();
        // }

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
