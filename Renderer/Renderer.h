#ifndef RENDERER
#define RENDERER

#include <GLES3/gl3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <random>
#include <cmath>

#include "../Scene/LightManager.h"

class Mesh;

using namespace std;

class Renderer
{
    public:

        Renderer(int width, int height, LightManager* lightManager);
        ~Renderer();

        void addMesh(Mesh* mesh, string fragShader);

        void render(mat4 &projection, mat4 &view);

    private:

        Shader* m_gBufferShader;
        Shader* m_deferredShader;
        Shader* m_ssaoShader;
        Shader* m_ssaoBlurShader;

        void initGBuffer();
        void initSSAO();

        Shaders resolveShader(string input);

        void sendTransforms(mat4 &projection, mat4 &view, mat4 model, GLuint programId);

        int m_width, m_height;

        unsigned int m_gBuffer, m_gPosition, m_gNormal, m_gColorSpec, m_ssaoFBO, m_ssaoColorBuffer, m_ssaoNoiseTexture, m_ssaoBlurFBO, m_ssaoBlurColorBuffer;

        vector<vec3> m_ssaoKernel;
        vector<vec3> m_ssaoNoise;

        vector<Mesh*> m_meshes;

        LightManager* m_lightManager;

        GLuint m_quadVboId;
        GLuint m_quadVaoId;
};

#endif