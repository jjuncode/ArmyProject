#include "TransformComponent.h"
#include "../pch.h"
#include "ColliderComponent.h"
#include "../Mgr/SceneMgr.h"

void TransformComponent::Update(float dt)
{
}

void TransformComponent::AddRotate(float offset)
{  
    m_rotate += offset;

    // Update OBB rotation
    auto collider = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    if (collider ){
        collider->RotateOBB(offset);
        auto obb = collider->GetOBB();
    }
}

constexpr Mat3 TransformComponent::GetModelMatrix() const
{
    // Create a transformation matrix
    Mat3 t{
        Vec3(1,0,m_pos.x),
        Vec3(0,1,m_pos.y),
        Vec3(0,0,1),
    };

    Mat3 s{
        Vec3(m_scale.x , 0, 0),
        Vec3(0, m_scale.y , 0),
        Vec3(0,0,1)
    };

    Mat3 r{
        Vec3(cos(m_rotate), -sin(m_rotate), 0),
        Vec3(sin(m_rotate), cos(m_rotate), 0),
        Vec3(0, 0, 1),
    };


    return t*r*s;
}