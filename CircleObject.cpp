#include "CircleObject.h"
#include "pch.h"
#include "Entity.h"
#include "SceneMgr.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "ColorComponent.h"

void CircleObject::Init()
{
    auto& cur_scene = SceneMgr::GetCurScene();

    Entity obj;
    auto render = obj.AddComponent<RenderComponent>();
    cur_scene->AddComponent<RenderComponent>(std::move(render));
    
    auto color = obj.AddComponent<ColorComponent>();
    color->SetColor(sf::Color::Red);
    cur_scene->AddComponent<ColorComponent>(std::move(color));
    
    auto transform = obj.AddComponent<TransformComponent>();
    transform->SetPos(Vec2(-25, 10 ));
    transform->SetScale(100);
    cur_scene->AddComponent<TransformComponent>(std::move(transform));
}