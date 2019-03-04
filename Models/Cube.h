#ifndef DEF_CUBE
#define DEF_CUBE
#define GLM_ENABLE_EXPERIMENTAL

#include <GLES3/gl3.h>

#ifndef BUFFER_OFFSET //vbo

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <cstring>

#include "../Shaders/Shader.h"

using namespace std;
using namespace glm;

class Cube
{
    public:

        Cube(float scale, string const vertexShader, string const fragmentShader);
        ~Cube();

        void draw(mat4 &projection, mat4 &modelview);

        void load();

        void loadVBO();
        void updateVBO(void *data, int size, int offset);

        void loadVAO();

    private:

        Shader m_shader;
        float m_vertex[108];
        float m_color[108];

        int m_vertexSize;
        int m_colorSize;

        GLuint m_vboId;
        GLuint m_vaoId;
};

#endif