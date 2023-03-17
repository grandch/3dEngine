#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

using namespace glm;

class Mesh;

class Bezier
{
    public:

        Bezier(vec3 p1, vec3 p2, vec3 pc1, vec3 pc2);
        ~Bezier();

        void addSegment(vec3 p2, vec3 pc1, vec3 pc2);

        void compute(int nbPoints);
        void draw(mat4 &projection, mat4 &modelview);

    private:

        vec3 b(float t);

        vec3 m_p1;
        vec3 m_p2;
        vec3 m_pc1;
        vec3 m_pc2;
        Mesh* m_meshCurve;
        Mesh* m_meshControlPolygon;

        Bezier* m_next;
};