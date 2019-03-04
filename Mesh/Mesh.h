#ifndef MESH
#define MESH

#include <GLES3/gl3.h>

#include <vector>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL

class MeshHalfEdge;
class MeshVertex;
class MeshTriangle;
class MeshEdge;

using namespace std;

class Mesh
{
    public:

        Mesh();
        ~Mesh();

        void loadVBO();

        GLuint makeFloatVBO(vector<GLfloat> values, int vboType, GLenum usage);
        GLuint makeShortVBO(vector<GLushort> values, int vboType, GLenum usage);

        void pushTriangle(MeshTriangle* triangle);
        void popTriangle(MeshTriangle* triangle);

        void pushHalfEdge(MeshHalfEdge* halfEdge);
        void popHalfEdge(MeshHalfEdge* halfEdge);

        void pushEdge(MeshEdge* edge);
        void popEdge(MeshEdge* edge);

        MeshVertex addVertex(string name);
        void pushVertex(MeshVertex* vertex);
        void popVertex(MeshVertex* vertex);

    private:

        vector<MeshTriangle*> m_triangleList;
        vector<MeshHalfEdge*> m_halfEdgeList;
        vector<MeshVertex*> m_vertexList;
        
        vector<MeshEdge*> m_edgeList;

        GLuint m_vertexVboId;
        GLuint m_colorVboId;
        GLuint m_indexVboId;
};
#endif