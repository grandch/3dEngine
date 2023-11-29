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
class MeshTriangle;
class Mesh;

class MeshVertex
{
    public:

        MeshVertex(Mesh* mesh, string name);
        ~MeshVertex();

        vector<MeshTriangle*> getTrianglesAround();

        int getNumber();
        void setNumber(int number);

        string getName();

        vec4 getAttribute(int index);
        vec2 getUv();
        MeshVertex* setCoord(vec3 coord);
        MeshVertex* setColor(vec3 color);
        MeshVertex* setNormal(vec3 normal);
        MeshVertex* setUv(vec2 uv);
        void computeNormal();

        MeshHalfEdge* getHalfEdgeTo(MeshVertex* vertex);

        void linkSibling(MeshHalfEdge* halfEdge);
        void unlinkSibling(MeshHalfEdge* halfEdge);

        // laplacian computing
        float getLaplacian();
        void setLaplacian(float laplacian);
        void applyLaplacian();

    private:

        string m_name;

        Mesh* m_mesh;

        vector<vec4> m_attributes;
        vec2 m_uv;

        MeshHalfEdge* m_halfEdge;
        vector<MeshTriangle*> m_faces;

        GLushort m_number;

        // laplacian computing
        float m_curentLaplacian;
        float m_computedLaplacian;
};
#endif