#include "PlayScene.h"
#include "pch.h"

#include "Object/Entity.h"
#include "Object/GridObject.h"
#include "Object/RectObject.h"
#include "Object/PlayerObject.h"

#include "Mgr/InputMgr.h"
#include "Mgr/SceneMgr.h"
#include "Mgr/CollisionMgr.h"

#include "Component/TransformComponent.h"

void PlayScene::Init()
{
    GridObject grid;
    grid.Init(gird_offset);

    PlayerObject player;
    int node = gird_offset;
    player.Init(Vec2(5*node,5*node), Vec2(gird_offset,gird_offset), sf::Color::Red);

    SetCollisionLayer(CollisionEntityType::kPlayer, CollisionEntityType::kWall, true);
}

void PlayScene::Update(float dt)
{
    // Entity Update
    for (const auto &entity : m_vec_component){
        if (entity){
            if (m_vec_status[entity->GetID()] == EntityStatus::kActive)
                entity->Update(dt);
        }
    }

    // Collision Check
   CollisionMgr::Collision();

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
            rect.Init(Vec2(mouse_pos.x, mouse_pos.y), Vec2(gird_offset,gird_offset), sf::Color::Yellow);
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