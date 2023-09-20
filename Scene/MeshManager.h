#ifndef MESHMANAGER
#define MESHMANAGER
#define GLM_ENABLE_EXPERIMENTAL

#include <map>

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

        void addMesh(char* name, Mesh* mesh);
        void removeMesh(char* name);
        void transformMesh(char* name, mat4 transform);

        void draw(mat4 &projection, mat4 &view, LightManager* lightManager);

    private:
        map<char*, Mesh*> m_meshes;
};

#endif