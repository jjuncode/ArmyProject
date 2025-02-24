#include "GridObject.h"
#include "../Core.h"
#include "../pch.h"
#include "Entity.h"
#include "../SceneMgr.h"
#include "../Component/RenderComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/ColorComponent.h"


void GridObject::Init(uint32_t offset)
{
    auto &cur_scene = SceneMgr::GetCurScene();

    // offset - 간격
    Vec2 window_size {Core::GetWindowSize()};
    for (int i=0; i<window_size.y/offset; ++i ) {
        for (int j=0; j<window_size.x/ offset; ++j ){
            Entity obj;
            auto render = obj.AddComponent<RenderComponent>();
            render->SetShape<sf::RectangleShape>();
            cur_scene->AddComponent<RenderComponent>(std::move(render));

            auto color = obj.AddComponent<ColorComponent>();
            color->SetColor(sf::Color::White);
            cur_scene->AddComponent<ColorComponent>(std::move(color));

            auto transform = obj.AddComponent<TransformComponent>();
            transform->SetPos(Vec2(i*offset,j*offset));
            transform->SetScale(Vec2(10,10));
            cur_scene->AddComponent<TransformComponent>(std::move(transform));
        }
    }
}