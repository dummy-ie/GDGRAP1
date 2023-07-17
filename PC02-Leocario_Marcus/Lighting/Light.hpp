#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gd
{
    using namespace glm;
    class Light
    {
    public:
        std::string shaderName;

        // vec3 ambient;
        // vec3 diffuse;
        // vec3 specular;

        float ambientStr;
        float specStr;
        float specPhong;

        vec3 lightColor;
        vec3 ambientColor;

    public:
        Light(std::string shaderName, float ambientStr, float specStr, float specPhong, vec3 lightColor, vec3 ambientColor) : 
            shaderName(shaderName),
            ambientStr(ambientStr), 
            specStr(specStr), 
            specPhong(specPhong), 
            lightColor(lightColor), 
            ambientColor(ambientColor) {};

        void applyUniforms(GLuint &shaderProgram)
        {
            // std::cout << "applying uniforms of " << shaderName << std::endl;
            
            unsigned int ambientStrLoc = glGetUniformLocation(shaderProgram, (shaderName + ".ambientStr").c_str());
            glUniform1f(ambientStrLoc, ambientStr);

            unsigned int specStrLoc = glGetUniformLocation(shaderProgram, (shaderName + ".specStr").c_str());
            glUniform1f(specStrLoc, specStr);
            
            unsigned int specPhongLoc = glGetUniformLocation(shaderProgram, (shaderName + ".specPhong").c_str());
            glUniform1f(specPhongLoc, specPhong);

            unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, (shaderName + ".lightColor").c_str());
            glUniform3fv(lightColorLoc, 1, value_ptr(lightColor));

            unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, (shaderName + ".ambientColor").c_str());
            glUniform3fv(ambientColorLoc, 1, value_ptr(ambientColor));
        }

        virtual void applyExtraUniforms(GLuint &shaderProgram) = 0;
    };
} // namespace gd

#endif // !LIGHT_HPP