#ifndef RENDERER
#define RENDERER

#include <GLES3/gl3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <vector>
#include <string>

#include "../Scene/LightManager.h"

class Mesh;

enum Shaders {
    blinnPhong,
    GGX,
    color,
    invalid
};

using namespace std;

class Renderer
{
    public:

        Renderer(int width, int height, LightManager* lightManager);
        ~Renderer();

        void addMesh(Mesh* mesh, string fragShader);
        void addShader(string fragShader, vector<Mesh*> meshes);

        void render(mat4 &projection, mat4 &view);

    private:

        Shader* m_gBufferShader; (to lock in the gbuffer pass)
        Shader* m_deferredShader; (to lock in the lighting pass (one for GGX and BlinnPhong or one shader per BRDF + direct rendering for color/transparency))

        void initGBuffer();

        Shaders resolveShader(string input);

        void sendTransforms(mat4 &projection, mat4 &view, mat4 model, GLuint programId);

        int m_width, m_height;

        unsigned int m_gBuffer, m_gPosition, m_gNormal, m_gColorSpec;

        map<Shaders, vector<Mesh*>> m_shaderDic;

        LightManager* m_lightManager;
};

#endif