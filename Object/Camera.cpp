#include "Camera.h"
#include "../Mgr/SceneMgr.h"

#include "../Component/RenderComponent.h"
#include "../Component/TransformComponent.h"

#include "../Script/CameraScript.h"


void Camera::SetScript()
{
    auto& cur_scene = SceneMgr::GetCurScene();
    auto script = obj.SetScript<CameraScript>();
    script->SetTarget(m_target);
    cur_scene ->AddScript<CameraScript>(std::move(script));
}

void Camera::CreateVertex(Vec2 _pos, Vec2 _scale)
{
}
