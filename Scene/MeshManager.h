#ifndef MESHMANAGER
#define MESHMANAGER
#define GLM_ENABLE_EXPERIMENTAL

#include <map>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Mesh/Mesh.h"
#include "LightManager.h"

class MeshManager
{
    public:
        MeshManager();
        ~MeshManager();

        void addMesh(string name, Mesh* mesh);
        Mesh* getMesh(string name);
        vector<pair<string, Mesh*>> getMeshes();
        void removeMesh(string name);
        void transformMesh(string name, mat4 transform);

        void draw(mat4 &projection, mat4 &view, LightManager* lightManager);

    private:
        map<string, Mesh*> m_meshes;
};

#endif