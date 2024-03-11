#include "Bone.h"
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>

Bone::Bone(): m_previous(nullptr)
{
    // first pose is always rest pose
    Pose pose;
    pose.translate = glm::vec3(0);
    pose.scale = glm::vec3(1);
    pose.rotate = glm::mat4(1);
    m_poses.push_back(pose);
}

Bone::Bone(Bone* previous): m_previous(previous)
{
    // first pose is always rest pose
    Pose pose;
    pose.translate = glm::vec3(0);
    pose.scale = glm::vec3(1);
    pose.rotate = glm::mat4(1);
    m_poses.push_back(pose);
}

Bone::~Bone() {}

glm::mat4 Bone::getPose(int idPose)
{
    if(idPose < m_poses.size())
    {
        glm::mat4 previousPose = glm::mat4(1);
        if( m_previous != nullptr)
        {
            previousPose = m_previous->getPose(idPose);
        }

        return previousPose * buildTransform(m_poses[idPose]);
    }
    // error
    return glm::mat4(1);
}

glm::mat4 Bone::getPose(int idPose1, int idPose2, float time)
{
    if(idPose1 < m_poses.size() && idPose2 < m_poses.size())
    {
        glm::mat4 previousPose = glm::mat4(1);
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
    return glm::mat4(1);
}

void Bone::setTranslate(int idPose, glm::vec3 transf)
{
    if(idPose < m_poses.size())
    {
        m_poses[idPose].translate = transf;
    }
    else {
        Pose pose;
        pose.scale = glm::vec3(1);
        pose.rotate = glm::mat4(1);
        pose.translate = transf;
        m_poses.push_back(pose);
    }
}

void Bone::setRotate(int idPose, glm::mat4 rot)
{
    if(idPose < m_poses.size())
    {
        m_poses[idPose].rotate = rot;
    }
    else {
        Pose pose;
        pose.translate = glm::vec3(0);
        pose.scale = glm::vec3(1);
        pose.rotate = rot;
        m_poses.push_back(pose);
    }
}

void Bone::setScale(int idPose, glm::vec3 sca)
{
    if(idPose < m_poses.size())
    {
        m_poses[idPose].scale = sca;
    }
    else {
        Pose pose;
        pose.translate = glm::vec3(0);
        pose.rotate = glm::mat4(1);
        pose.scale = sca;
        m_poses.push_back(pose);
    }
}

glm::mat4 Bone::buildTransform(Pose pose)
{
    glm::mat4 transf = glm::translate(glm::mat4(1), pose.translate);
    transf = pose.rotate * transf;
    transf = glm::scale(transf, pose.scale);

    return transf;
}