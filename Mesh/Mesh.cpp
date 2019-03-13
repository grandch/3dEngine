#include "Mesh.h"

#include "MeshHalfEdge.h"
#include "MeshVertex.h"
#include "MeshTriangle.h"
#include "MeshEdge.h"

Mesh::Mesh(string vertexShader, string fragmentShader): m_shader(vertexShader, fragmentShader)
{
    MeshVertex* v1 = new MeshVertex(this, "v1");
    v1->setCoord(vec3(-0.5, 0.5, 0.5));

    MeshVertex* v2 = new MeshVertex(this, "v2");
    v2->setCoord(vec3(0.5, -0.5, 0.5));

    MeshVertex* v3 = new MeshVertex(this, "v3");
    v3->setCoord(vec3(0.5, 0.5, -0.5));

    MeshVertex* v4 = new MeshVertex(this, "v4");
    v4->setCoord(vec3(-0.5, -0.5, -0.5));

    
    //random colors
    v1->setColor(vec3(1.0, 1.0, 1.0));
    v2->setColor(vec3(1.0, 1.0, 1.0));
    v3->setColor(vec3(1.0, 1.0, 1.0));
    v4->setColor(vec3(1.0, 1.0, 1.0));

    MeshTriangle* t1 = new MeshTriangle(this, v1, v2, v3);

    MeshTriangle* t2 = new MeshTriangle(this, v1, v3, v4);

    MeshTriangle* t3 = new MeshTriangle(this, v1, v2, v4);

    MeshTriangle* t4 = new MeshTriangle(this, v2, v3, v4);

    loadVBO();
    loadVAO();
    loadEdgeVAO();

    m_shader.load();
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vertexVboId);
    glDeleteBuffers(1, &m_colorVboId);
    glDeleteBuffers(1, &m_faceIndexVboId);
    glDeleteBuffers(1, &m_edgeIndexVboId);
}


void Mesh::draw(mat4 &projection, mat4 &modelview)
{
    glUseProgram(m_shader.getProgramID());

        glBindVertexArray(m_vaoId); //lock the vao

            //send matrix to shaders
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
            
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0); //unlock the vao

        glBindVertexArray(m_edgeVaoId);

            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));

            glDrawElements(GL_LINES, 12, GL_UNSIGNED_SHORT, 0);

        glBindVertexArray(0);

    glUseProgram(0);
}       

void Mesh::loadVBO()
{
    //array to send
    vector<GLfloat> vertex;
    vector<GLfloat> colors;

    int number = 0;

    for(MeshVertex* top: m_vertexList)
    {
        top->setNumber(number); //location in the vbo (index)
        number++;

        vertex.push_back(top->getAttribute(0)[0]);
        vertex.push_back(top->getAttribute(0)[1]);
        vertex.push_back(top->getAttribute(0)[2]);

        colors.push_back(top->getAttribute(1)[0]);
        colors.push_back(top->getAttribute(1)[1]);
        colors.push_back(top->getAttribute(1)[2]);
    }

    vector<GLushort> faceIndexList;

    for(MeshTriangle* triangle: m_triangleList)
    {
        MeshHalfEdge* halfEdge = triangle->getHalfEdge();
        faceIndexList.push_back(halfEdge->getOrigin()->getNumber());

        halfEdge = halfEdge->getNext();
        faceIndexList.push_back(halfEdge->getOrigin()->getNumber());

        halfEdge = halfEdge->getNext();
        faceIndexList.push_back(halfEdge->getOrigin()->getNumber());
    }

    vector<GLushort> edgeIndexList;

    for(MeshEdge* edge: m_edgeList)
    {
        edgeIndexList.push_back(edge->getVertex(0)->getNumber());
        edgeIndexList.push_back(edge->getVertex(1)->getNumber());
    }

    m_vertexVboId = makeFloatVBO(vertex, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    m_colorVboId = makeFloatVBO(colors, GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    m_faceIndexVboId = makeShortVBO(faceIndexList, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    m_edgeIndexVboId = makeShortVBO(edgeIndexList, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
}

void Mesh::loadVAO()
{
    glGenVertexArrays(1, &m_vaoId);

    glBindVertexArray(m_vaoId);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, m_colorVboId);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_faceIndexVboId);
        
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::loadEdgeVAO()
{
    glGenVertexArrays(1, &m_edgeVaoId);

    glBindVertexArray(m_edgeVaoId);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_edgeIndexVboId);
        
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint Mesh::makeFloatVBO(vector<GLfloat> values, int vboType, GLenum usage)
{
    GLuint id;

    glGenBuffers(1, &id);

    glBindBuffer(vboType, id);

        glBufferData(vboType, values.size()*sizeof(GLfloat), values.data(), usage);

    glBindBuffer(vboType, 0);

    return id;
}

GLuint Mesh::makeShortVBO(vector<GLushort> values, int vboType, GLenum usage)
{
    GLuint id;

    glGenBuffers(1, &id);

    glBindBuffer(vboType, id);

        glBufferData(vboType, values.size()*sizeof(GLushort), values.data(), usage);

    glBindBuffer(vboType, 0);

    return id;
}

MeshTriangle* Mesh::addTriangle(MeshVertex* v1, MeshVertex* v2, MeshVertex* v3)
{
    MeshTriangle* triangle = new MeshTriangle(this, v1, v2, v3);
    return triangle;
}

void Mesh::pushTriangle(MeshTriangle* triangle)
{
    m_triangleList.push_back(triangle);
}

void Mesh::popTriangle(MeshTriangle* triangle)
{
    int i = 0;

    while(m_triangleList[i] != triangle)
    {
        i++;
    }

    m_triangleList.erase(m_triangleList.begin() + i);
}

void Mesh::pushHalfEdge(MeshHalfEdge* halfEdge)
{
    m_halfEdgeList.push_back(halfEdge);
}

void Mesh::popHalfEdge(MeshHalfEdge* halfEdge)
{
    int i = 0;

    while(m_halfEdgeList[i] != halfEdge)
    {
        i++;
    }

    m_halfEdgeList.erase(m_halfEdgeList.begin() + i);
}

void Mesh::pushEdge(MeshEdge* edge)
{
    bool exist = false;

    for(MeshEdge* line : m_edgeList)
    {
        if((line->getVertex(0) == edge->getVertex(0) && line->getVertex(1) == edge->getVertex(1)) || (line->getVertex(1) == edge->getVertex(0) && line->getVertex(0) == edge->getVertex(1)))
        {
            exist = true;
        }
    }

    if(!exist)
    {
        m_edgeList.push_back(edge);
    }
}

void Mesh::popEdge(MeshEdge* edge)
{
    int i = 0;

    while(m_edgeList[i] != edge)
    {
        i++;
    }

    m_edgeList.erase(m_edgeList.begin() + i);
}

MeshVertex* Mesh::addVertex(string name)
{
    MeshVertex* vertex = new MeshVertex(this, name);
    return vertex;
}

void Mesh::pushVertex(MeshVertex* vertex)
{
    m_vertexList.push_back(vertex);
}

void Mesh::popVertex(MeshVertex* vertex)
{
    int i = 0;

    while(m_vertexList[i] != vertex)
    {
        i++;
    }

    m_vertexList.erase(m_vertexList.begin() + i);
}