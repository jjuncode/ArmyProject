#include "Camera.h"
#include "../Mgr/SceneMgr.h"
#include "../Component/RenderComponent.h"
#include "../Component/CameraScript.h"
#include "../Component/TransformComponent.h"


void Camera::SetScript()
{
    auto& cur_scene = SceneMgr::GetCurScene();
    auto script = obj.AddComponent<CameraScript>();
    script->SetTarget(m_target);
    cur_scene ->AddComponent<CameraScript>(std::move(script));
}

void Camera::CreateVertex(Vec2 _pos, Vec2 _scale)
{
}
