#include "RectObject.h"
#include "../pch.h"
#include "Entity.h"
#include "../SceneMgr.h"
#include "../Component/RenderComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/ColorComponent.h"

void RectObject::Init(Vec2 _pos, Vec2 _scale, sf::Color _color)
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
}