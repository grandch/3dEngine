#include "Bone.h"
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>

#include <iostream>

Bone::Bone(glm::vec3 position): m_position(position), m_previous(nullptr)
{
    // first pose is always rest pose
    Pose pose;
    pose.translate = glm::vec3(0);
    pose.scale = glm::vec3(1);
    pose.rotate = glm::mat4(1);
    m_poses.push_back(pose);
    m_transf.push_back(buildTransform(pose));
}

Bone::Bone(glm::vec3 position, Bone* previous): m_position(position), m_previous(previous)
{
    // first pose is always rest pose
    Pose pose;
    pose.translate = glm::vec3(0);
    pose.scale = glm::vec3(1);
    pose.rotate = glm::mat4(1);
    m_poses.push_back(pose);
    m_transf.push_back(buildTransform(pose));
}

Bone::~Bone() {}

glm::mat4 Bone::getPose(int idPose)
{
    if(idPose < m_poses.size())
        return m_transf[idPose];
    // error
    return glm::mat4(1);
}

glm::mat4 Bone::getPose(int idPose1, int idPose2, float time)
{
    time = 0.5 * time + 0.5;
    if(idPose1 < m_poses.size() && idPose2 < m_poses.size())
    {
        glm::mat4 previousPose = glm::mat4(1);
        // if( m_previous != nullptr)
        // {
        //     previousPose = m_previous->getPose(idPose1, idPose2, time);
        // }

        Pose pose;
        pose.translate = time * m_poses[idPose1].translate + (1 - time) * m_poses[idPose2].translate;
        pose.rotate = time * m_poses[idPose1].rotate + (1 - time) * m_poses[idPose2].rotate;
        pose.scale = time * m_poses[idPose1].scale + (1 - time) * m_poses[idPose2].scale;

        return previousPose * buildTransform(pose);
    }
    // error
    return glm::mat4(1);
}

glm::vec4 Bone::tranformPointWithPose(glm::vec4 point, int idPose1, int idPose2, float time)
{
    if(m_previous != nullptr)
        point = m_previous->tranformPointWithPose(point, idPose1, idPose2, time);
    point = glm::translate(-m_position) * point;
    glm::mat4 pose = getPose(idPose1, idPose2, time);
    point = pose * point;
    return glm::translate(m_position) * point;
}

void Bone::setTranslate(int idPose, glm::vec3 transf)
{
    if(idPose < m_poses.size())
    {
        m_poses[idPose].translate = transf;
        m_transf[idPose] = getPreviousPose(idPose) * buildTransform(m_poses[idPose]);
    }
    else {
        Pose pose;
        pose.scale = glm::vec3(1);
        pose.rotate = glm::mat4(1);
        pose.translate = transf;
        m_poses.push_back(pose);
        m_transf.push_back(getPreviousPose(idPose) * buildTransform(pose));
    }
}

void Bone::setRotate(int idPose, glm::mat4 rot)
{
    if(idPose < m_poses.size())
    {
        m_poses[idPose].rotate = rot;
        m_transf[idPose] = getPreviousPose(idPose) * buildTransform(m_poses[idPose]);
    }
    else {
        Pose pose;
        pose.translate = glm::vec3(0);
        pose.scale = glm::vec3(1);
        pose.rotate = rot;
        m_poses.push_back(pose);
        m_transf.push_back(getPreviousPose(idPose) * buildTransform(pose));
    }
}

void Bone::setScale(int idPose, glm::vec3 sca)
{
    if(idPose < m_poses.size())
    {
        m_poses[idPose].scale = sca;
        m_transf[idPose] = getPreviousPose(idPose) * buildTransform(m_poses[idPose]);
    }
    else {
        Pose pose;
        pose.translate = glm::vec3(0);
        pose.rotate = glm::mat4(1);
        pose.scale = sca;
        m_poses.push_back(pose);
        m_transf.push_back(getPreviousPose(idPose) * buildTransform(pose));
    }
}

glm::mat4 Bone::buildTransform(Pose pose)
{
    glm::mat4 transf = glm::scale(glm::mat4(1), pose.scale);
    transf = pose.rotate * transf;
    transf = glm::translate(transf, pose.translate);
    
    return transf;
}

glm::mat4 Bone::getPreviousPose(int idPose)
{
    if( m_previous != nullptr)
        return m_previous->getPose(idPose);
    return glm::mat4(1);
}