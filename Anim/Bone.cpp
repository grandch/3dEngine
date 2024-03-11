#include "Bone.h"

Bone::Bone(Bone* previous): m_previous(previous)
{
    // first pose is always rest pose
    Pose pose;
    pose.translate = vec3(0);
    pose.scale = vec3(0);
    pose.rotate = rotate(0.f, vec3(1, 0, 0));
    m_poses.push_back(pose);
}

Bone::~Bone() {}

mat4 Bone::getPose(int idPose)
{
    if(idPose < m_poses.size())
    {
        mat4 previousPose = mat4(1);
        if( m_previous != nullptr)
        {
            previousPose = m_previous->getPose(idPose);
        }

        return previousPose * buildTransform(m_poses[idPose]);
    }
    // error
    return mat4(1);
}

mat4 Bone::getPose(int idPose1, int idPose2, float time)
{
    if(idPose1 < m_poses.size() && idPose2 < m_poses.size())
    {
        mat4 previousPose = mat4(1);
        if( m_previous != nullptr)
        {
            previousPose = m_previous->getPose(idPose1, idPose2, time);
        }

        Pose pose;
        pose.translate = time * m_poses[idPose1].translate + (1 - time) * m_poses[idPose2].translate;
        pose.rotate = time * m_poses[idPose1].rotate + (1 - time) * m_poses[idPose2].rotate;
        pose.scale = time * m_poses[idPose1].scale + (1 - time) * m_poses[idPose2].scale;

        return previousPose * buildTransform(pose);
    }
    // error
    return mat4(1);
}

// void Bone::setPose(int idPose, mat4 pose)
// {
//     if(idPose < m_poses.size())
//     {
//         m_poses[idPose] = pose;
//     }
//     else {
//         m_poses.push_back(pose);
//     }
// }

mat4 Bone::buildTransform(Pose pose)
{
    mat4 transf = translate(mat4(1), pose.translate);
    transf = pose.rotate * transf;
    transf = scale(transf, pose.scale);

    return transf;
}