#include "Transform.h"
#include "../pch.h"
#include "../Component/ColliderComponent.h"
#include "../Mgr/SceneMgr.h"
   
void Transform::AddRotate(Vec3 _offset)
{
    //  // Update OBB rotation
    // auto collider = SceneMgr::GetComponent<ColliderComponent>(m_owner_id);
    // if (collider ){
    //     collider->RotateOBB(offset);
    //     auto obb = collider->GetOBB();
    // }

    // pitch / yaw / roll
    _offset.x = Vec::GetRadian(_offset.x);
    _offset.y = Vec::GetRadian(_offset.y);
    _offset.z = Vec::GetRadian(_offset.z);

    m_rotate += _offset;

    if (m_rotate.x > 360.f ) m_rotate.x -= 360.f;
    if (m_rotate.y > 360.f ) m_rotate.y -= 360.f;
    if (m_rotate.z > 360.f ) m_rotate.z -= 360.f;

    if ( m_rotate.x < 0.f ) m_rotate.x += 360.f;
    if ( m_rotate.y < 0.f ) m_rotate.y += 360.f;
    if ( m_rotate.z < 0.f ) m_rotate.z += 360.f;

    Quaternion quaternion{};
    quaternion.CreateQuaternion(m_rotate);

    m_right = quaternion.RotateVector(Vec3(1,0,0));
    m_up = quaternion.RotateVector(Vec3(0,1,0));
    m_forward = quaternion.RotateVector(Vec3(0,0,1));
}

const Mat4 Transform::GetModelMatrix() const
{
    // Create a transformation matrix
    Mat4 t{
        Vec4(1,0,0,m_pos.x),
        Vec4(0,1,0,m_pos.y),
        Vec4(0,0,1,m_pos.z),
        Vec4(0,0,0,1)
    };

    Mat4 s{
        Vec4(m_scale.x, 0, 0, 0),
        Vec4(0, m_scale.y, 0, 0),
        Vec4(0, 0, m_scale.z, 0),
        Vec4(0, 0, 0, 1)
    };

    Mat4 r{
        Vec4(m_right.x, m_up.x, m_forward.x ),
        Vec4(m_right.y, m_up.y, m_forward.y ),
        Vec4(m_right.z, m_up.z, m_forward.z ),
        Vec4(0, 0, 0, 1)
    };


    return t*r*s;
}