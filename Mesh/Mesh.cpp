#include "Mesh.h"

#include "MeshHalfEdge.h"
#include "MeshVertex.h"
#include "MeshTriangle.h"
#include "MeshEdge.h"

Mesh::Mesh(string vertexShader, string fragmentShader): m_shader(vertexShader, fragmentShader)
{
    m_vertex.push_back(vec3(-0.5, 0.5, 0.5));
    m_vertex.push_back(vec3(-0.5, -0.5, 0.5));
    m_vertex.push_back(vec3(0.5, -0.5, 0.5));
    m_vertex.push_back(vec3(0.5, 0.5, 0.5));
    m_vertex.push_back(vec3(-0.5, 0.5, -0.5));
    m_vertex.push_back(vec3(-0.5, -0.5, -0.5));
    m_vertex.push_back(vec3(0.5, -0.5, -0.5));
    m_vertex.push_back(vec3(0.5, 0.5, -0.5));
    
    //random colors
    m_color.push_back(vec3(0.0, 0.0, 1.0));
    m_color.push_back(vec3(1.0, 0.0, 0.0));
    m_color.push_back(vec3(0.0, 1.0, 0.0));
    m_color.push_back(vec3(1.0, 0.0, 0.0));
    m_color.push_back(vec3(0.0, 1.0, 0.0));
    m_color.push_back(vec3(0.0, 0.0, 1.0));
    m_color.push_back(vec3(0.0, 1.0, 0.0));
    m_color.push_back(vec3(0.0, 0.0, 1.0));

    m_shader.load();
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vertexVboId);
    glDeleteBuffers(1, &m_colorVboId);
    glDeleteBuffers(1, &m_indexVboId);
}


void Mesh::draw(mat4 &projection, mat4 &modelview)
{
    glUseProgram(m_shader.getProgramID());

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexVboId);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, m_colorVboId);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVboId);

            //send matrix to shaders
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(0);
}

void Mesh::loadTempVBO()
{
    vector<GLfloat> vertex;
    vector<GLfloat> color;
    vector<GLushort> index;

    for(vec3 top: m_vertex)
    {
        vertex.push_back(top.x);
        vertex.push_back(top.y);
        vertex.push_back(top.z);
    }
    for(vec3 col: m_color)
    {
        color.push_back(col.x);
        color.push_back(col.y);
        color.push_back(col.z);
    }

    //ATTENTION DEGEULASSE
    index.push_back(0);
    index.push_back(1);
    index.push_back(3);
    index.push_back(1);
    index.push_back(3);
    index.push_back(2);

    index.push_back(1);
    index.push_back(2);
    index.push_back(5);
    index.push_back(2);
    index.push_back(5);
    index.push_back(6);

    index.push_back(0);
    index.push_back(1);
    index.push_back(5);
    index.push_back(0);
    index.push_back(5);
    index.push_back(4);
    
    index.push_back(0);
    index.push_back(3);
    index.push_back(4);
    index.push_back(3);
    index.push_back(4);
    index.push_back(7);
    
    index.push_back(3);
    index.push_back(2);
    index.push_back(6);
    index.push_back(3);
    index.push_back(6);
    index.push_back(7);
    
    index.push_back(4);
    index.push_back(5);
    index.push_back(6);
    index.push_back(4);
    index.push_back(6);
    index.push_back(7);


    m_vertexVboId = makeFloatVBO(vertex, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    m_colorVboId = makeFloatVBO(color, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    m_indexVboId = makeShortVBO(index, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
}

void Mesh::loadVBO()
{
    //array to send
    vector<GLfloat> vertex;
    vector<GLfloat> colors;

    int number = 0;

    for(MeshVertex* top: m_vertexList)
    {
        top->setNumber(number); //location in the vbo
        number++;

        //vertex
        vertex.push_back(top->getAttribute(0)[0]);
        vertex.push_back(top->getAttribute(0)[1]);
        vertex.push_back(top->getAttribute(0)[2]);

        //color
        colors.push_back(top->getAttribute(1)[0]);
        colors.push_back(top->getAttribute(1)[1]);
        colors.push_back(top->getAttribute(1)[2]);
    }

    vector<GLushort> indexList;

    for(MeshTriangle* triangle: m_triangleList)
    {
        MeshHalfEdge* halfEdge = triangle->getHalfEdge();
        indexList.push_back(halfEdge->getOrigin()->getNumber());

        halfEdge = halfEdge->getNext();
        indexList.push_back(halfEdge->getOrigin()->getNumber());

        halfEdge = halfEdge->getNext();
        indexList.push_back(halfEdge->getOrigin()->getNumber());
    }

    m_vertexVboId = makeFloatVBO(vertex, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    m_colorVboId = makeFloatVBO(colors, GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    m_indexVboId = makeShortVBO(indexList, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
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
    m_edgeList.push_back(edge);
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

MeshVertex Mesh::addVertex(string name)
{
    MeshVertex vertex(this, name);
    return vertex;
}

void Mesh::pushVertex(MeshVertex* vertex)
{
    m_vertexList.push_back(vertex);
}

void Mesh::popVertex(MeshVertex* vertex)
{
    int i = 0;

    while(m_vertexList[i] != vertex) //search the vertex in the list
    {
        i++;
    }

    m_vertexList.erase(m_vertexList.begin() + i); //remove it
}