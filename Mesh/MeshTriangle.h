#ifndef MESHTRIANGLE
#define MESHTRIANGLE

#include <GLES3/gl3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MeshHalfEdge;
class Mesh;
class MeshVertex;

using namespace glm;

class MeshTriangle
{
    public:

        MeshTriangle(Mesh* mesh, MeshVertex* v1, MeshVertex* v2, MeshVertex* v3);
        ~MeshTriangle();

        MeshHalfEdge* getHalfEdge();

        MeshVertex* getVertex0();
        MeshVertex* getVertex1();
        MeshVertex* getVertex2();

        vec3 getNormal();
    
    private:

        MeshHalfEdge* m_halfEdge;
        vec3 m_normal;

        Mesh* m_mesh;
};
#endif