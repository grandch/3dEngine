#ifndef CAMERAROTATEAROUND
#define CAMERAROTATEAROUND

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Inputs/Input.h"
#include "Camera.h"

using namespace glm;

class CameraRotateAround : Camera
{
    public:

        CameraRotateAround();
        CameraRotateAround(vec3 position, vec3 target, vec3 verticalAxis);
        ~CameraRotateAround();

        void orientate(int xRel, int yRel);
        void move(Input const &input);

    private:
};

#endif