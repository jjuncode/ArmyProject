#include "PlayScene.h"
#include "pch.h"

#include "Object/Entity.h"
#include "Object/GridObject.h"
#include "Object/RectObject.h"
#include "Object/PlayerObject.h"
#include "Object/Polygon.h"
#include "Object/Camera.h"

#include "Mgr/InputMgr.h"
#include "Mgr/SceneMgr.h"
#include "Mgr/CollisionMgr.h"

#include "Component/TransformComponent.h"

void PlayScene::Init()
{
    // GridObject grid;
    // grid.Init(gird_offset);

    int size = 100;

    PlayerObject player{Vec2(50,50), Vec2(size,size)};
    player.SetCollider(CollisionEntityType::kPlayer);

    Camera player_camera{player.GetEntityID()};
    player_camera.SetMainCamera();


    Polygon poly{12,Vec2(4*size,4*size), Vec2(size,size)};
    poly.SetCollider(CollisionEntityType::kWall);

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

    Vec2 offset{static_cast<float>(static_cast<int>(mouse_pos.x) % static_cast<int>(gird_offset)),
        static_cast<float>(static_cast<int>(mouse_pos.y) % static_cast<int>(gird_offset))};
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
            RectObject rect{Vec2(mouse_pos.x, mouse_pos.y), Vec2(gird_offset,gird_offset)};
            rect.SetCollider(CollisionEntityType::kWall);
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
                        auto& comp = SceneMgr::GetComponent(id);
                        SceneMgr::DeleteComponent(std::move(comp));
                    }
                    std::cout << "DELETE" << std::endl;
                    break;
                }
            }
        }
    }
}