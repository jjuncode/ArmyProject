#include "GridObject.h"
#include "../Core.h"
#include "../pch.h"
#include "Entity.h"
#include "../Mgr/SceneMgr.h"
#include "../Component/RenderComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/ColorComponent.h"


void GridObject::Init(uint32_t offset)
{
    auto &cur_scene = SceneMgr::GetCurScene();

    // offset - 간격
    Vec2 window_size {Core::GetWindowSize()};

    // 가로줄 
    for(int i=0; i<window_size.y/offset; ++i){
        Entity obj;
        auto render = obj.AddComponent<RenderComponent>();
        render->SetShape<sf::RectangleShape>(sf::Vector2f(1, 1));
        cur_scene->AddComponent<RenderComponent>(std::move(render));

        auto color = obj.AddComponent<ColorComponent>();
        color->SetColor(sf::Color::White);
        cur_scene->AddComponent<ColorComponent>(std::move(color));

        auto transform = obj.AddComponent<TransformComponent>();
        transform->SetPos(Vec2(0 ,i* offset -2.5f));
        transform->SetScale(Vec2(window_size.x, 5));
        cur_scene->AddComponent<TransformComponent>(std::move(transform));
    }

    // 세로줄
    for(int i=0; i<window_size.x/offset; ++i){
        Entity obj;
        auto render = obj.AddComponent<RenderComponent>();
        render->SetShape<sf::RectangleShape>(sf::Vector2f(1, 1));
        cur_scene->AddComponent<RenderComponent>(std::move(render));

        auto color = obj.AddComponent<ColorComponent>();
        color->SetColor(sf::Color::White);
        cur_scene->AddComponent<ColorComponent>(std::move(color));

        auto transform = obj.AddComponent<TransformComponent>();
        transform->SetPos(Vec2(i* offset -2.5f,0));
        transform->SetScale(Vec2(5,window_size.y));
        cur_scene->AddComponent<TransformComponent>(std::move(transform));
    }
}