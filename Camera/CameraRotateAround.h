#ifndef CAMERAROTATEAROUND
#define CAMERAROTATEAROUND

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Inputs/Input.h"
#include "Camera.h"

using namespace glm;

class CameraRotateAround : public Camera
{
    public:

        CameraRotateAround();
        CameraRotateAround(vec3 position, vec3 target, vec3 up);
        ~CameraRotateAround();

        void orientate(int xRel, int yRel);
        void translate(int xRel, int yRel);
        void zoom(int y);
        void move(Input const &input);

    private:
};

#endif