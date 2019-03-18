#ifndef CAMERA
#define CAMERA

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Inputs/Input.h"

using namespace glm;

class Camera
{
    public:

        Camera();
        Camera(vec3 position, vec3 target, vec3 verticalAxis);
        ~Camera();

        void orientate(int xRel, int yRel);
        void move(Input const &input);
        void lookAt(mat4 &modelview);

    private:

        float m_phi, m_theta;

        vec3 m_orientation;

        vec3 m_verticalAxis;
        vec3 m_lateralAxis;

        vec3 m_position;
        vec3 m_target;
};

#endif