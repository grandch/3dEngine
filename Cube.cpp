#include "Cube.h"

Cube::Cube(float scale, string const vertexShader, string const fragmentShader): 
m_shader(vertexShader, fragmentShader)
{
    m_shader.charger();

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
{}

void Cube::draw(mat4 &projection, mat4 &modelview)
{
    glUseProgram(m_shader.getProgramID()); //load the current shader


            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertex); //first vertexAttrib (0), takes three coord (3)
            glEnableVertexAttribArray(0); //load the first vertexAttrib

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, m_color); //same
            glEnableVertexAttribArray(1); //same

            //send matrix to shaders
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));

            glDrawArrays(GL_TRIANGLES, 0, 36); //draw the m_vertex

            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(0);

        glUseProgram(0); //desable the shader
}