#ifndef MESHTRIANGLE
#define MESHTRIANGLE

#include <GLES3/gl3.h>

class MeshHalfEdge;
class Mesh;
class MeshVertex;

class MeshTriangle
{
    public:

        MeshTriangle(Mesh* mesh, MeshVertex* v1, MeshVertex* v2, MeshVertex* v3);
        ~MeshTriangle();

        MeshHalfEdge* getHalfEdge();

        MeshVertex* getVertex0();
        MeshVertex* getVertex1();
        MeshVertex* getVertex2();
    
    private:

        MeshHalfEdge* m_halfEdge;

        Mesh* m_mesh;
};
#endif