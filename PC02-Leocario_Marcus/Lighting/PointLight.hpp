#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "Light.hpp"

namespace gd
{
    class PointLight : public Light
    {
    public:
        vec3 position;
        float constant = 1.0f;
        float linear = 0.045f;
        float quadratic = 0.0075f;

    public:
        PointLight(std::string shaderName, vec3 position, float ambientStr, float specStr, float specPhong, vec3 lightColor, vec3 ambientColor) : 
        position(position), Light(shaderName, ambientStr, specStr, specPhong, lightColor, ambientColor) {}

        void applyExtraUniforms(GLuint &shaderProgram)
        {
            // std::cout << "applying extra uniforms of " << shaderName << std::endl;

            unsigned int positionLoc = glGetUniformLocation(shaderProgram, (shaderName + ".position").c_str());
            glUniform3fv(positionLoc, 1, value_ptr(position));

            unsigned int constantLoc = glGetUniformLocation(shaderProgram, (shaderName + ".constant").c_str());
            glUniform1f(constantLoc, constant);

            unsigned int linearLoc = glGetUniformLocation(shaderProgram, (shaderName + ".linear").c_str());
            glUniform1f(linearLoc, linear);
            
            unsigned int quadraticLoc = glGetUniformLocation(shaderProgram, (shaderName + ".quadratic").c_str());
            glUniform1f(quadraticLoc, quadratic);
        }
    };    
} // namespace gd


#endif // !POINT_LIGHT_HPP