#include "PlayerObject.h"
#include "../Mgr/SceneMgr.h"
#include "../Component/RenderComponent.h"
#include "../Component/TransformComponent.h"
#include "../Script/PlayerScript.h"

void PlayerObject::CreateVertex(Vec2 _pos, Vec2 _scale)
{
    // Rectangle
    // 4 dot
    auto scale_half = _scale/2;
    auto transform = SceneMgr::GetComponent<TransformComponent>(obj.GetEntityID());

    Vec2 left_bot = Vec2(-scale_half.x, -scale_half.y);
    Vec2 left_top = Vec2(-scale_half.x , scale_half.y);
    Vec2 right_top = Vec2(scale_half.x, scale_half.y );
    Vec2 right_bot = Vec2(scale_half.x, -scale_half.y);

    transform->SetVertexs({left_bot,left_top,right_top,right_bot});
}
