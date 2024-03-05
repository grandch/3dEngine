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

        Renderer(LightManager* lightManager);
        ~Renderer();

        void addMesh(Mesh* mesh, string fragShader);
        void addShader(string fragShader, vector<Mesh*> meshes);

        void render(mat4 &projection, mat4 &view);

    private:

        Shaders resolveShader(string input);

        map<Shaders, vector<Mesh*>> m_shaderDic;

        LightManager* m_lightManager;
};

#endif