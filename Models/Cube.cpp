#include "Cube.h"

Cube::Cube(float scale, string const vertexShader, string const fragmentShader): 
shader(vertexShader, fragmentShader), vertexSize(108*sizeof(float)), colorSize(108*sizeof(float)), vboID(0)
{
    shader.load();

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
        vertex[i] = vertexTmp[i];
        color[i] = colorTmp[i]; 
    }
}

Cube::~Cube()
{
    glDeleteBuffers(1, &vboID);
}


void Cube::draw(mat4 &projection, mat4 &modelview)
{
    glUseProgram(shader.getProgramID()); //load the current shader

            glBindBuffer(GL_ARRAY_BUFFER, vboID); //lock the vbo

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); //first vertexAttrib (0), takes three coord (3)
                glEnableVertexAttribArray(0); //load the first vertexAttrib

                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexSize)); //same
                glEnableVertexAttribArray(1); //same

                //send matrix to shaders
                glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
                glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));

                glDrawArrays(GL_TRIANGLES, 0, 36); //draw the vertex

                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0); //unlock the vbo

        glUseProgram(0); //desable the shader
}

void Cube::load()
{
    if(glIsBuffer(vboID) == GL_TRUE ) //delete the current vbo
    {
        glDeleteBuffers(1, &vboID);
    }

    glGenBuffers(1, &vboID); //create the ID

    glBindBuffer(GL_ARRAY_BUFFER, vboID); //lock the vbo

        glBufferData(GL_ARRAY_BUFFER, vertexSize + colorSize, 0, GL_STATIC_DRAW); //memory allocation

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vertex);
        glBufferSubData(GL_ARRAY_BUFFER, vertexSize, colorSize, color);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //unlock the vbo
}

void Cube::updateVbo(void *data, int size, int offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

        void *vbo = glMapBufferRange(GL_ARRAY_BUFFER, 0, vertexSize + colorSize, GL_MAP_WRITE_BIT);

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