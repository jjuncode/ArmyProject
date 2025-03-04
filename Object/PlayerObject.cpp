#include "PlayerObject.h"
#include "../pch.h"
#include "Entity.h"
#include "../Mgr/SceneMgr.h"
#include "../Component/RenderComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/ColorComponent.h"

void PlayerObject::Init(Vec2 _pos, Vec2 _scale, sf::Color _color)
{
    auto& cur_scene = SceneMgr::GetCurScene();

    Entity obj;
    auto render = obj.AddComponent<RenderComponent>();
    render->SetShape<sf::RectangleShape>(sf::Vector2f(1,1));
    cur_scene->AddComponent<RenderComponent>(std::move(render));
    
    auto color = obj.AddComponent<ColorComponent>();
    color->SetColor(_color);
    cur_scene->AddComponent<ColorComponent>(std::move(color));
    
    auto transform = obj.AddComponent<TransformComponent>();
    transform->SetPos(_pos);
    transform->SetScale(_scale);
    cur_scene->AddComponent<TransformComponent>(std::move(transform));

    SceneMgr::AddEntity<PlayerObject>(obj);
}