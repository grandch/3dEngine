#include "../Mesh/Mesh.h"
#include <vector>

using namespace std;

class BezierCurve;

class BezierSurface
{
    public:

        BezierSurface(BezierCurve* bc0, BezierCurve* bc1, BezierCurve* bc2, BezierCurve* bc3);
        ~BezierSurface();

        void compute(int s, int t, string vertexShader, string fragmentShader);
        void draw(mat4 &projection, mat4 &view);

        void transform(mat4 transf);

        Mesh* getMesh();
    
    private:

        vector<BezierCurve*> m_bcT;
        vector<BezierCurve*> m_bcS;

        Mesh* m_mesh;
};