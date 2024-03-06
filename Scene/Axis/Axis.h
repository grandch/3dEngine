#include "../../Mesh/Mesh.h"
#include "../../Renderer/Renderer.h"
#include "../../Shaders/Material.h"

class Axis
{
    public:

        Axis(Renderer* renderer);
        ~Axis();
    
    private:

        Mesh* m_meshX;
        Mesh* m_meshY;
        Mesh* m_meshZ;

        Material* m_material;
};