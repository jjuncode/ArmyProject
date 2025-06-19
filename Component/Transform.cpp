#include "Transform.h"
#include "../pch.h"
#include "ColliderComponent.h"
#include "../Mgr/SceneMgr.h"

void Transform::AddRotate(float offset)
{  
    m_rotate += offset;

    // Update OBB rotation
    auto collider = SceneMgr::GetComponent<ColliderComponent>(m_owner_id);
    if (collider ){
        collider->RotateOBB(offset);
        auto obb = collider->GetOBB();
    }
}

const Mat3 Transform::GetModelMatrix() const
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