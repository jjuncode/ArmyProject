#include "PlayerObject.h"
#include "../Mgr/SceneMgr.h"
#include "../Component/RenderComponent.h"
#include "../Component/PlayerScript.h"
#include "../Component/TransformComponent.h"

void PlayerObject::SetScript()
{
    auto& cur_scene = SceneMgr::GetCurScene();
    auto script = obj.AddComponent<PlayerScript>();
    cur_scene ->AddComponent<PlayerScript>(std::move(script));
}

void PlayerObject::CreateVertex(Vec2 _pos, Vec2 _scale)
{
    // Rectangle
    // 4 dot
    auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(obj.GetEntityID());

    Vec2 left_bot = _pos + Vec2(-_scale.x/2, -_scale.y/2);
    Vec2 left_top = _pos + Vec2(-_scale.x/2 , _scale.y/2);
    Vec2 right_top = _pos + Vec2(_scale.x/2, _scale.y/2);
    Vec2 right_bot = _pos + Vec2(_scale.x/2, -_scale.y/2);

    transform->SetVertexs({left_bot,left_top,right_top,right_bot});
    transform->CreateEdge(); 
}
