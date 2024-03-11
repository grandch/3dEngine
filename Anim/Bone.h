#ifndef BONE
#define BONE

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace glm;
using namespace std;

typedef struct 
{
    vec3 translate;
    vec3 scale;
    mat4 rotate;
} Pose;

class Bone
{
    public:

        Bone(Bone* previous);
        ~Bone();

        mat4 getPose(int idPose);
        mat4 getPose(int idPose1, int idPose2, float time);

        // void setPose(int idPose, mat4 pose);

    private:

        mat4 buildTransform(Pose pose);

        Bone* m_previous;
        vector<Pose> m_poses;
};

#endif