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
#include "../Shaders/Material.h"
#include "../Scene/LightManager.h"
#include "../Renderer/Renderer.h"

class MeshHalfEdge;
class MeshVertex;
class MeshTriangle;
class MeshEdge;

using namespace std;
using namespace glm;

class Mesh
{
    public:

        Mesh(string vertexShader, string fragmentShader, Material* material, Renderer* renderer);
        ~Mesh();

        void loadMesh(); //load the shader and the vbo/vao

        bool displayEdges();

        void loadVBO();
        void loadVAO();
        GLuint getVaoId();
        GLuint getEdgeVaoId();

        void loadEdgeVAO();

        GLuint makeFloatVBO(vector<GLfloat> values, int vboType, GLenum usage);
        GLuint makeShortVBO(vector<GLushort> values, int vboType, GLenum usage);

        void addPolygon(vector<MeshVertex*> vertex);

        MeshTriangle* addTriangle(MeshVertex* v1, MeshVertex* v2, MeshVertex* v3);
        void pushTriangle(MeshTriangle* triangle);
        void popTriangle(MeshTriangle* triangle);
        int getTrianglesNb();

        void pushHalfEdge(MeshHalfEdge* halfEdge);
        void popHalfEdge(MeshHalfEdge* halfEdge);

        void pushEdge(MeshEdge* edge);
        void popEdge(MeshEdge* edge);
        int getEdgesNb();

        MeshVertex* addVertex(string name);
        void pushVertex(MeshVertex* vertex);
        void popVertex(MeshVertex* vertex);

        void computeNormals();
        void compileShaders();

        void setDrawEdges(bool de);

        Shader* getShader();
        Material* getMaterial();

        void transform(mat4 transform);
        mat4 getModelTransform();

    private:

        mat4 m_model;

        vector<MeshTriangle*> m_triangleList;
        vector<MeshHalfEdge*> m_halfEdgeList;
        vector<MeshVertex*> m_vertexList;
        vector<MeshEdge*> m_edgeList;

        Shader m_shader;
        Material* m_material;

        GLuint m_vertexVboId;
        GLuint m_colorVboId;
        GLuint m_normalVboId;
        GLuint m_uvVboId;

        GLuint m_faceIndexVboId;
        GLuint m_edgeIndexVboId;

        GLuint m_vaoId;
        GLuint m_edgeVaoId;

        bool m_drawEdges;
};
#endif