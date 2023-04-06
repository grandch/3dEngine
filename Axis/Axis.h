#include "../Mesh/Mesh.h"

class Axis
{
    public:

        Axis();
        ~Axis();

        void loadAxis();
        void draw(mat4 &projection, mat4 &view);
    
    private:

    Mesh* m_meshX;
    Mesh* m_meshY;
    Mesh* m_meshZ;
};