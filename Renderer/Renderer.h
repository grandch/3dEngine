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

class Mesh;

using namespace std;

class Renderer
{
    public:
        Renderer();
        ~Renderer();

        void addMesh(Mesh* mesh, string fragShader);
        void addShader(string fragShader, vector<Mesh*> meshes);

        void render();

    private:
        map<string, vector<Mesh*>> m_shaderDic;
};

#endif