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

    RectObject rect;
    
    Vec2 offset{mouse_pos.x % static_cast<int>(gird_offset) , mouse_pos.y %static_cast<int>(gird_offset) };
    mouse_pos -= offset;
    
    // 0~40 -> 0 , 40~ 80 -> 40 ... 
    if (mouse_state == MouseState::kLeftTap || mouse_state == MouseState::kLeftHold){

        bool same{false};
        
        auto vec_id = SceneMgr::GetEntityVector<RectObject>();
        for(const auto& id: vec_id){
            auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(id);
            if ( transform ) {
                auto pos = transform->GetPos();
                if (mouse_pos == pos){
                    same = true;
                    break;
                }
            }
        }

        if ( !same ){
            rect.Init(Vec2(mouse_pos.x, mouse_pos.y), Vec2(gird_offset,gird_offset), sf::Color::Cyan);
        }

    }
    else if (mouse_state == MouseState::kRightTap || mouse_state == MouseState::kRightHold){
        auto vec_id = SceneMgr::GetEntityVector<RectObject>();
        for(const auto& id: vec_id){
            auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(id);
            if ( transform ) {
                auto pos = transform->GetPos();
                if (mouse_pos == pos){
                    auto vec_id = SceneMgr::GetComponentsID(transform->GetOwnerID());
                    for (const auto& id : vec_id){
                        auto comp = SceneMgr::GetComponent(id);
                        SceneMgr::DeleteComponent(std::move(comp));
                    }
                    std::cout << "DELETE" << std::endl;
                    break;
                }
            }
        }
    }
}