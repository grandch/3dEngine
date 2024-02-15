#ifndef CAMERA
#define CAMERA

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Inputs/Input.h"

class Camera
{
    public:
        Camera() = default;
        virtual ~Camera() {};

        virtual void orientate(int xRel, int yRel) = 0;
        virtual void move(Input const &input) = 0;
        inline void lookAt(glm::mat4 &view) {view = glm::lookAt(m_position, m_target, m_up);}

    protected:
        glm::vec3 m_position;
        glm::vec3 m_target;
        glm::vec3 m_up;
        glm::vec3 m_orientation;
        glm::vec3 m_right;
};

#endif