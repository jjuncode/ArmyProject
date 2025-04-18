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
    auto camera_transform = SceneMgr::GetComponentOrigin<TransformComponent>(m_target);
    return camera_transform->GetPos();
}

void CameraScript::FollowTargetPos()
{
    auto target_transform = SceneMgr::GetComponentOrigin<TransformComponent>(m_target);
    auto camera_transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
    
    camera_transform->SetPos(target_transform->GetPos());
}
