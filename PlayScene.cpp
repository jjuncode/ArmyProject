#include "PlayScene.h"
#include "pch.h"
#include "Object/CircleObject.h"
#include "Object/GridObject.h"
#include "Object/RectObject.h"
#include "Mgr/InputMgr.h"
#include "Mgr/SceneMgr.h"
#include "Object/Entity.h"
#include "Component/TransformComponent.h"

void PlayScene::Init()
{
    CircleObject circle;
    circle.Init(Vec2(25,50),10, sf::Color::Cyan);

    CircleObject circle2;
    circle2.Init(Vec2(50,100), 10, sf::Color::Red);

    CircleObject circle3;
    circle3.Init(Vec2(70,70), 10, sf::Color::White);

    GridObject grid;
    grid.Init(gird_offset);

    RectObject rect;
    rect.Init(Vec2(60-20,60-20), Vec2(gird_offset,gird_offset)
            , sf::Color::Red);
}

void PlayScene::Update(float dt)
{
    auto mouse_state = InputMgr::GetMouseState();
    auto mouse_pos = InputMgr::GetMousePos();

    if (mouse_state == MouseState::kLeftTap || mouse_state == MouseState::kLeftHold){
        RectObject rect;
        auto rect_pos = mouse_pos;
        
        Vec2 offset{rect_pos.x % static_cast<int>(gird_offset) , rect_pos.y %static_cast<int>(gird_offset) };
        rect_pos -= offset;
        // 0~40 -> 0 , 40~ 80 -> 40 ... 

        bool same{false};
        
        auto vec_id = SceneMgr::GetEntityVector<RectObject>();
        for(const auto& id: vec_id){
            auto transform = SceneMgr::GetComponent<TransformComponent>(id);
            auto pos = transform->GetPos();
            if ( rect_pos == pos ) {
                same= true;
                break;
            }
        }

        if ( !same ){
            rect.Init(Vec2(rect_pos.x, rect_pos.y), Vec2(gird_offset,gird_offset), sf::Color::Cyan);
        }

    }
}