#include "CircleObject.h"
#include "../pch.h"
#include "Entity.h"
#include "../Mgr/SceneMgr.h"
#include "../Component/RenderComponent.h"
#include "../Component/TransformComponent.h"

void CircleObject::Init(Vec2 _pos, uint32_t _scale, sf::Color _color)
{
    auto& cur_scene = SceneMgr::GetCurScene();

    Entity obj;
    auto render = obj.AddComponent<RenderComponent>();
    cur_scene->AddComponent<RenderComponent>(std::move(render));
    
    auto transform = obj.AddComponent<TransformComponent>();
    transform->SetPos(_pos);
    transform->SetScale(Vec2(_scale,_scale));
    cur_scene->AddComponent<TransformComponent>(std::move(transform));
}