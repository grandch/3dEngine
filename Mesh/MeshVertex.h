#ifndef MESHVERTEX
#define MESHVERTEX

#include <GLES3/gl3.h>
#include <string>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

class MeshHalfEdge;
class Mesh;

class MeshVertex
{
    public:

        MeshVertex(Mesh* mesh, string name);
        ~MeshVertex();

        void processTrianglesAround1();
        void processTrianglesAround2();

        int getNumber();
        void setNumber(int number);

        string getName();

        vec4 getAttribute(int index);
        MeshVertex* setCoord(vec3 coord);
        MeshVertex* setColor(vec3 color);
        MeshVertex* setNormal(vec3 normal);

        MeshHalfEdge* getHalfEdgeTo(MeshVertex* vertex);

        void linkSibling(MeshHalfEdge* halfEdge);
        void unlinkSibling(MeshHalfEdge* halfEdge);

    private:

        vector<vec4> m_attributes;

        MeshHalfEdge* m_halfEdge;
        GLushort m_number;
        Mesh* m_mesh;

        string m_name;
};
#endif