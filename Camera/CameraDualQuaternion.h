#ifndef CAMERADUALQUATERNION
#define CAMERADUALQUATERNION

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Inputs/Input.h"
#include "Camera.h"

using namespace glm;

class CameraDualQuaternion : public Camera
{
    public:

        CameraDualQuaternion();
        CameraDualQuaternion(vec3 position, vec3 target, vec3 up);
        ~CameraDualQuaternion();

        void orientate(int xRel, int yRel);
        void move(Input const &input);

    private:

        float m_phi, m_theta;
};

#endif