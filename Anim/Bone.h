#ifndef BONE
#define BONE

#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

typedef struct 
{
    glm::vec3 translate;
    glm::vec3 scale;
    glm::mat4 rotate;
} Pose;

class Bone
{
    public:

        Bone();
        Bone(Bone* previous);
        ~Bone();

        glm::mat4 getPose(int idPose);
        glm::mat4 getPose(int idPose1, int idPose2, float time);

        void setTranslate(int idPose, glm::vec3 transf);
        void setRotate(int idPose, glm::mat4 rot);
        void setScale(int idPose, glm::vec3 sca);

    private:

        glm::mat4 buildTransform(Pose pose);

        Bone* m_previous;
        vector<Pose> m_poses;
};

#endif