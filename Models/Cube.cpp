#include "Cube.h"

Cube::Cube(float scale, string const vertexShader, string const fragmentShader): 
m_shader(vertexShader, fragmentShader), m_vertexSize(108*sizeof(float)), m_colorSize(108*sizeof(float)), m_vboId(0), m_vaoId(0)
{
    m_shader.load();

    scale /= 2;
    float vertexTmp[] = {-scale, -scale, -scale,   scale, -scale, -scale,   scale, scale, -scale,
                        -scale, -scale, -scale,   -scale, scale, -scale,   scale, scale, -scale,     // Face 1

                       scale, -scale, scale,   scale, -scale, -scale,   scale, scale, -scale,
                       scale, -scale, scale,   scale, scale, scale,   scale, scale, -scale,         // Face 2

                       -scale, -scale, scale,   scale, -scale, scale,   scale, -scale, -scale,
                       -scale, -scale, scale,   -scale, -scale, -scale,   scale, -scale, -scale,    // Face 3

                       -scale, -scale, scale,   scale, -scale, scale,   scale, scale, scale,
                       -scale, -scale, scale,   -scale, scale, scale,   scale, scale, scale,        // Face 4

                       -scale, -scale, -scale,   -scale, -scale, scale,   -scale, scale, scale,
                       -scale, -scale, -scale,   -scale, scale, -scale,   -scale, scale, scale,     // Face 5

                       -scale, scale, scale,   scale, scale, scale,   scale, scale, -scale,
                       -scale, scale, scale,   -scale, scale, -scale,   scale, scale, -scale};      // Face 6

    float colorTmp[] = {1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,
                       1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1

                       0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,
                       0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2

                       0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,
                       0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3

                       1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,
                       1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 4

                       0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,
                       0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 5

                       0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,
                       0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0};          // Face 6

    for(int i = 0; i < 108; i++)
    {
        m_vertex[i] = vertexTmp[i];
        m_color[i] = colorTmp[i]; 
    }
}

Cube::~Cube()
{
    glDeleteBuffers(1, &m_vboId);
    glDeleteVertexArrays(1, &m_vaoId);
}


void Cube::draw(mat4 &projection, mat4 &view)
{
    glUseProgram(m_shader.getProgramID()); //load the current shader

        glBindVertexArray(m_vaoId); //lock the vao

            //send matrix to shaders
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "view"), 1, GL_FALSE, value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));

            glDrawArrays(GL_TRIANGLES, 0, 36); //draw the vertex

        glBindVertexArray(0); //unlock the vao        

    glUseProgram(0); //desable the shader
}

void Cube::load()
{
    loadVBO();
    loadVAO();
}

void Cube::loadVBO()
{
    if(glIsBuffer(m_vboId) == GL_TRUE ) //delete the current vbo
    {
        glDeleteBuffers(1, &m_vboId);
    }

    glGenBuffers(1, &m_vboId); //create an ID

    glBindBuffer(GL_ARRAY_BUFFER, m_vboId); //lock the vbo

        glBufferData(GL_ARRAY_BUFFER, m_vertexSize + m_colorSize, 0, GL_STATIC_DRAW); //memory allocation

        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexSize, m_vertex);
        glBufferSubData(GL_ARRAY_BUFFER, m_vertexSize, m_colorSize, m_color);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //unlock the vbo
}

void Cube::updateVBO(void *data, int size, int offset)
{
    if(glIsVertexArray(m_vaoId) == GL_TRUE)
    {
        glDeleteVertexArrays(1, &m_vaoId);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

        void *vbo = glMapBufferRange(GL_ARRAY_BUFFER, 0, m_vertexSize + m_colorSize, GL_MAP_WRITE_BIT);

        if(vbo == NULL)
        {
            cout << "Error while loading vbo" << endl;
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            return;
        }

        memcpy((char*)vbo + offset, data, size);

        glUnmapBuffer(GL_ARRAY_BUFFER);
        vbo = 0;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::loadVAO()
{
    glGenVertexArrays(1, &m_vaoId);

    glBindVertexArray(m_vaoId);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboId); //lock the vbo

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); //first vertexAttrib (0), vbo with no offset
            glEnableVertexAttribArray(0); //load the first vertexAttrib

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_vertexSize)); //second vertexAttrib, vbo with vertexSizeOffset
            glEnableVertexAttribArray(1); //same

        glBindBuffer(GL_ARRAY_BUFFER, 0); //unlock the vbo

    glBindVertexArray(0);
}