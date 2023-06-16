#include "Model3D.hpp"

using namespace model;
using namespace glm;

// insert constructor variables into attributes
Model3D::Model3D(vec3 pos, vec3 rot, vec3 scale) : position(pos), rotation(rot), scale(scale)
{
}

void Model3D::draw(GLuint &shaderProgram, GLuint &VAO, std::vector<GLuint> &mesh_indices)
{
    // generate a transformation matrix based on the stored attributes for this model3d
    glm::mat4 identity_matrix4 = glm::mat4(1.f);
    glm::mat4 transformation_matrix = glm::translate(identity_matrix4, position);
    transformation_matrix = glm::scale(transformation_matrix, scale);
    transformation_matrix = glm::rotate(transformation_matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    transformation_matrix = glm::rotate(transformation_matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    transformation_matrix = glm::rotate(transformation_matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));

    // send to the given shaderprogram
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    // draw
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(
        GL_TRIANGLES,
        mesh_indices.size(),
        GL_UNSIGNED_INT,
        0);
}
