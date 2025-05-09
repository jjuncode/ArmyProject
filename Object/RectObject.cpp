#include "RectObject.h"
#include "../Mgr/SceneMgr.h"
#include "../Component/TransformComponent.h"

void RectObject::CreateVertex(Vec2 _pos,Vec2 _scale)
{
    // Rectangle
    // 4 dot
    auto transform = SceneMgr::GetComponent<TransformComponent>(obj.GetEntityID());

    Vec2 left_bot = _pos - Vec2(_scale.x/2, _scale.y/2);
    Vec2 left_top = _pos + Vec2(-_scale.x/2, _scale.y/2);
    Vec2 right_top = _pos + Vec2(_scale.x/2, _scale.y/2);
    Vec2 right_bot = _pos + Vec2(_scale.x/2, -_scale.y/2);

    transform->SetVertexs({left_bot,left_top,right_top,right_bot});
}
