#include "CameraScript.h"
#include "TransformComponent.h"

#include "../Object/PlayerObject.h"
#include "../Mgr/SceneMgr.h"
#include "../Core.h"

void CameraScript::Update(float dt)
{
    auto target_transform = SceneMgr::GetComponentOrigin<TransformComponent>(m_target);
    auto camera_transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
    
    camera_transform->SetPos(target_transform->GetPos());

    auto resolution = Core::GetWindowSize();

    auto camera_pos = camera_transform->GetPos() - resolution/2;
    camera_transform->SetPos(camera_pos);
}