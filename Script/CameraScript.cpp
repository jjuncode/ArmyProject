#include "CameraScript.h"

#include "../Mgr/SceneMgr.h"
#include "../Mgr/InputMgr.h"

#include "../Core.h"

void CameraScript::Execute(float dt)
{
   // FollowTargetPos(dt);
}

Vec3 CameraScript::GetMainCameraPos()
{
    auto &camera_transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();
    return camera_transform.GetPos();
}

void CameraScript::FollowTargetPos(float dt)
{
    auto &target_transform = SceneMgr::GetObject(m_target).GetTransform();
    auto& camera_transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();

    auto target_pos = target_transform.GetPos();
    auto camera_pos = camera_transform.GetPos();

    auto distn = target_pos - camera_pos;
    if ( Vec::LengthSquare(distn) < 1.f ) {
        camera_pos = target_pos;
    }
    else{
        auto dir = Vec::Normalize(distn);
        camera_pos += dir* dt * m_speed;
    }

    camera_transform.SetPos(camera_pos);
}

const Mat4 CameraScript::GetViewMatrix() const
{
    auto& transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();

    auto pos = transform.GetPos();

    Mat4 t_inverse{
        Vec4(1, 0, 0, -pos.x),
        Vec4(0, 1, 0, -pos.y),
        Vec4(0, 0, 1, -pos.z),
        Vec4(0, 0, 0, 1),
    };

    // Mat4 r_inverse{
    //     Vec3(cos(rotate), sin(rotate), 0),
    //     Vec3(-sin(rotate), cos(rotate), 0),
    //     Vec3(0, 0, 1),
    // };

    // return r_inverse * t_inverse; // View Matrix
    return t_inverse;
}

const Mat4 CameraScript::GetProjectionMatrix() const
{
    float focal_length = 1/tanf(Vec::GetRadian(m_fov * 0.5f));
    auto window = Core::GetWindowSize();
    auto aspect = window.x / window.y;

    // Mat4 projection_matrix {
    //     Vec4( focal_length/aspect, 0, 0, 0),
    //     Vec4(0,  focal_length, 0, 0),
    //     Vec4( 0, 0, -1, 0),
    //     Vec4(0, 0, 0, 1)
    // };

    auto denom = m_near - m_far;

    Mat4 projection_matrix {
        Vec4( focal_length/aspect, 0, 0, 0),
        Vec4(0,  focal_length, 0, 0),
        Vec4( 0, 0, (m_near+m_far) / denom, 2*m_near*m_far / denom),
        Vec4(0, 0, -1, 0)
    };

    return projection_matrix;
}
