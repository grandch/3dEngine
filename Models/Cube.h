#ifndef DEF_CUBE
#define DEF_CUBE
#define GLM_ENABLE_EXPERIMENTAL

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GLES3/gl3.h>

#endif

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
        void updateVbo(void *data, int size, int offset);

    private:

        Shader shader;
        float vertex[108];
        float color[108];

        int vertexSize, colorSize;

        GLuint vboID;
};

#endif