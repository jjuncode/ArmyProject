#include "CameraScript.h"
#include "TransformComponent.h"

#include "../Object/PlayerObject.h"

#include "../Mgr/SceneMgr.h"
#include "../Mgr/InputMgr.h"

#include "../Core.h"

void CameraScript::Update(float dt)
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
}

void CameraScript::FollowTargetPos()
{
    auto target_transform = SceneMgr::GetComponentOrigin<TransformComponent>(m_target);
    auto camera_transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
    
    camera_transform->SetPos(target_transform->GetPos());

     // auto resolution = Core::GetWindowSize();
    // auto camera_pos = camera_transform->GetPos() - resolution/2;
    // camera_transform->SetPos(camera_pos);

}
