#ifndef MODEL_3D
#define MODEL_3D

namespace model
{
    using namespace glm;
    class Model3D
    {
    private:
        vec3 position = vec3(0.f);
        vec3 scale = vec3(1.f);
        vec3 rotation = vec3(0.f);
        // float theta_mod1 = 0;
        // float theta_mod2 = 0;
        // vec4 rgba_mod = vec4(1.0f, 0.72f, 0.77f, 1.0f);

    public:
        Model3D(vec3 pos = vec3(0.f, -1.f, 10.f), vec3 rot = vec3(0.f), vec3 sca = vec3(1.f));
        ~Model3D() = default;

        void draw(GLuint &shaderProgram, GLuint &VAO, std::vector<GLuint> &mesh_indices);
    };
} // namespace model

#endif // !MODEL_3D