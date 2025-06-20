#include "CameraScript.h"

#include "../Mgr/SceneMgr.h"
#include "../Mgr/InputMgr.h"

#include "../Core.h"

void CameraScript::Execute(float dt)
{
    FollowTargetPos(dt);
}

Vec2 CameraScript::GetMainCameraPos()
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

const Mat3 CameraScript::GetViewMatrix() const
{
    auto& transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();

    Vec2 pos = transform.GetPos();
    float rotate = transform.GetRotate();

    Mat3 t_inverse{
        Vec3(1, 0, -pos.x),
        Vec3(0, 1, -pos.y),
        Vec3(0, 0, 1),
    };
    Mat3 r_inverse{
        Vec3(cos(rotate), sin(rotate), 0),
        Vec3(-sin(rotate), cos(rotate), 0),
        Vec3(0, 0, 1),
    };

    return r_inverse * t_inverse; // View Matrix
}
