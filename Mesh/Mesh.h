#ifndef MESH
#define MESH

#include <GLES3/gl3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "../Shaders/Shader.h"

//#define GLM_ENABLE_EXPERIMENTAL

class MeshHalfEdge;
class MeshVertex;
class MeshTriangle;
class MeshEdge;

using namespace std;
using namespace glm;

class Mesh
{
    public:

        Mesh(string vertexShader, string fragmentShader);
        ~Mesh();

        void loadMesh();

        void draw(mat4 &projection, mat4 &modelview);

        void loadVBO();
        void loadVAO();

        void loadEdgeVAO();

        GLuint makeFloatVBO(vector<GLfloat> values, int vboType, GLenum usage);
        GLuint makeShortVBO(vector<GLushort> values, int vboType, GLenum usage);

        MeshTriangle* addTriangle(MeshVertex* v1, MeshVertex* v2, MeshVertex* v3);
        void pushTriangle(MeshTriangle* triangle);
        void popTriangle(MeshTriangle* triangle);

        void pushHalfEdge(MeshHalfEdge* halfEdge);
        void popHalfEdge(MeshHalfEdge* halfEdge);

        void pushEdge(MeshEdge* edge);
        void popEdge(MeshEdge* edge);

        MeshVertex* addVertex(string name);
        void pushVertex(MeshVertex* vertex);
        void popVertex(MeshVertex* vertex);

    private:

        vector<MeshTriangle*> m_triangleList;
        vector<MeshHalfEdge*> m_halfEdgeList;
        vector<MeshVertex*> m_vertexList;
        vector<MeshEdge*> m_edgeList;

        Shader m_shader;

        GLuint m_vertexVboId;
        GLuint m_colorVboId;
        GLuint m_faceIndexVboId;
        GLuint m_edgeIndexVboId;

        GLuint m_vaoId;
        GLuint m_edgeVaoId;
};
#endif