#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>

using namespace glm;
using namespace std;

class Mesh;

class BezierCurve
{
    public:

        BezierCurve(vec3 p0, vec3 p1, vec3 p2, vec3 p3);
        ~BezierCurve();

        vec3 getPoint(int i);

        vec3 b(float t);

        void addSegment(vec3 p1, vec3 p2, vec3 p3);

        vector<vec3> compute(int nbPoints);
        void draw(mat4 &projection, mat4 &view);

    private:

        vec3 m_p0;
        vec3 m_p3;
        vec3 m_p1;
        vec3 m_p2;
        Mesh* m_meshCurve;
        Mesh* m_meshControlPolygon;

        BezierCurve* m_next;
};