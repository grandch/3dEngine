#include "../../Mesh/Mesh.h"
#include "../../Renderer/Renderer.h"

class Axis
{
    public:

        Axis(Renderer* renderer);
        ~Axis();

        void draw(mat4 &projection, mat4 &view);
    
    private:

        Mesh* m_meshX;
        Mesh* m_meshY;
        Mesh* m_meshZ;
};