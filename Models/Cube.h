#ifndef DEF_CUBE
#define DEF_CUBE
#define GLM_ENABLE_EXPERIMENTAL

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GLES3/gl3.h>

#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "../Shaders/Shader.h"

using namespace std;
using namespace glm;

class Cube
{
    public:

        Cube(float scale, string const vertexShader, string const fragmentShader);
        ~Cube();

        void draw(mat4 &projection, mat4 &modelview);

    private:

        Shader m_shader;
        float m_vertex[108];
        float m_color[108];

};

#endif