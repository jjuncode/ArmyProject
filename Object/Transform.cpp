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
    m_rotate += _offset;

    if (m_rotate.x > 360.f ) m_rotate.x -= 360.f;
    if (m_rotate.y > 360.f ) m_rotate.y -= 360.f;
    if (m_rotate.z > 360.f ) m_rotate.z -= 360.f;

    if ( m_rotate.x < 0.f ) m_rotate.x += 360.f;
    if ( m_rotate.y < 0.f ) m_rotate.y += 360.f;
    if ( m_rotate.z < 0.f ) m_rotate.z += 360.f;

    float cos_pitch = cos(Vec::GetRadian(m_rotate.x));
    float sin_pitch = sin(Vec::GetRadian(m_rotate.x));

    float cos_yaw = cos(Vec::GetRadian(m_rotate.y));
    float sin_yaw = sin(Vec::GetRadian(m_rotate.y));

    float cos_roll = cos(Vec::GetRadian( m_rotate.z) );
    float sin_roll = sin(Vec::GetRadian( m_rotate.z) );

    // 오른손 좌표계, Yaw → Pitch → Roll 기준 방향 벡터
    m_right = Vec3(
        cos_yaw * cos_roll + sin_yaw * sin_pitch * sin_roll,
        cos_pitch * sin_roll,
        -sin_yaw * cos_roll + cos_yaw * sin_pitch * sin_roll);

    m_up = Vec3(
        -cos_yaw * sin_roll + sin_yaw * sin_pitch * cos_roll,
        cos_pitch * cos_roll,
        sin_yaw * sin_roll + cos_yaw * sin_pitch * cos_roll);

    m_forward = Vec3(
        sin_yaw * cos_pitch,
        -sin_pitch,
        cos_yaw * cos_pitch);
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