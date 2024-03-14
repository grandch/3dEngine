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

        Bone(glm::vec3 position);
        Bone(glm::vec3 position, Bone* previous);
        ~Bone();

        glm::mat4 getPose(int idPose);
        glm::mat4 getPose(int idPose1, int idPose2, float time);

        glm::vec4 tranformPointWithPose(glm::vec4 point, int idPose1, int idPose2, float time);

        void setTranslate(int idPose, glm::vec3 transf);
        void setRotate(int idPose, glm::mat4 rot);
        void setScale(int idPose, glm::vec3 sca);

    private:

        glm::mat4 buildTransform(Pose pose);
        glm::mat4 getPreviousPose(int idPose);

        Bone* m_previous;
        vector<Pose> m_poses;
        vector<glm::mat4> m_transf;

        glm::vec3 m_position;
};

#endif