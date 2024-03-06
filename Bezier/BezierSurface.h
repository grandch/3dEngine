#ifndef BEZIERSURFACE
#define BEZIERSURFACE

#include "../Mesh/Mesh.h"
#include "../Scene/LightManager.h"
#include "../Renderer/Renderer.h"
#include "../Shaders/Material.h"

#include <vector>

using namespace std;

class BezierCurve;

class BezierSurface
{
    public:

        BezierSurface(BezierCurve* bc0, BezierCurve* bc1, BezierCurve* bc2, BezierCurve* bc3, Renderer* renderer);
        ~BezierSurface();

        void compute(int s, int t, string vertexShader, string fragmentShader);

        void transform(mat4 transf);

        Mesh* getMesh();
    
    private:

        vector<BezierCurve*> m_bcT;
        vector<BezierCurve*> m_bcS;

        Mesh* m_mesh;

        Renderer* m_renderer;

        Material* m_material;
};

#endif