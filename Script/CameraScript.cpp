#include "CameraScript.h"
#include "../Component/TransformComponent.h"

#include "../Mgr/SceneMgr.h"
#include "../Mgr/InputMgr.h"

#include "../Core.h"

void CameraScript::Execute(float dt)
{
    FollowTargetPos();

    if ( InputMgr::IsHold(sf::Keyboard::Equal)){
        // Zoom in 
        if ( m_zoom_value < m_zoom_max) 
           m_zoom_value += m_zoom_speed * dt;
        else {
            m_zoom_value = m_zoom_max;
        }    
    }
    else if ( InputMgr::IsHold(sf::Keyboard::Dash)){
        // Zoom out
        if ( m_zoom_value > m_zoom_min ){
            m_zoom_value -= m_zoom_speed * dt;
        }
        else {
            m_zoom_value = m_zoom_min;
        }
    }

    if ( InputMgr::IsTap(sf::Keyboard::Num0)){
        m_zoom_value = 1.0f;
    }
}

Vec2 CameraScript::GetMainCameraPos()
{
    auto camera_transform = SceneMgr::GetComponent<TransformComponent>(m_target);
    return camera_transform->GetPos();
}

void CameraScript::FollowTargetPos()
{
    auto target_transform = SceneMgr::GetComponent<TransformComponent>(m_target);
    auto camera_transform = SceneMgr::GetComponent<TransformComponent>(GetOwnerID());
    
    camera_transform->SetPos(target_transform->GetPos());
}

const Mat3 CameraScript::GetViewMatrix() const
{
    auto transform = SceneMgr::GetComponent<TransformComponent>(m_target);

    if (!transform) {
        return Mat3();
    }

    Vec2 pos = transform->GetPos();
    float rotate = transform->GetRotate();

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
