#include "PlayScene.h"
#include "pch.h"
#include "Core.h"

#include "Object/Entity.h"
#include "Object/RectObject.h"
#include "Object/PlayerObject.h"
#include "Object/Polygon.h"
#include "Object/Camera.h"

#include "Mgr/InputMgr.h"
#include "Mgr/SceneMgr.h"
#include "Mgr/CollisionMgr.h"

#include "Component/TransformComponent.h"
#include "Component/RigidbodyComponent.h"

#include "Script/CameraScript.h"
#include "Script/PlayerScript.h"
#include "Script/WallScript.h"

void PlayScene::Init()
{
    // GridObject grid;
    // grid.Init(gird_offset);

    int size = 100;

    PlayerObject player{Vec2(50,50), Vec2(size,size)};
    player.SetCollider(CollisionEntityType::kPlayer, Vec2(size,size));
    player.AddComponent<Rigidbody>(1.f,0.9f, 0.85f);
    player.SetScript<PlayerScript>();

    Camera player_camera{player.GetEntityID()};
    player_camera.SetScript<CameraScript>(player.GetEntityID());
    player_camera.SetMainCamera();


    Polygon poly{12,Vec2(2*size,2*size), Vec2(size,size)};
    poly.SetCollider(CollisionEntityType::kWall, Vec2(size,size));
    poly.SetScript<WallScript>();
    poly.AddComponent<Rigidbody>(10.f, 0.9f, 0.85f);

    
    Polygon poly2{12,Vec2(4.5f*size,4.5f*size), Vec2(size,size)};
    poly2.SetCollider(CollisionEntityType::kWall, Vec2(size,size));
    poly2.SetScript<WallScript>();
    poly2.AddComponent<Rigidbody>(10.f, 0.9f, 0.85f);


    SetCollisionLayer(CollisionEntityType::kPlayer, CollisionEntityType::kWall, true);
    SetCollisionLayer(CollisionEntityType::kWall, CollisionEntityType::kWall, true);
}

void PlayScene::Update(float dt)
{
    // Entity Update
    for (const auto &entity : m_vec_component){
        if (entity){
            if (m_vec_entity_status[entity->GetOwnerID()] == EntityStatus::kActive)
                entity->Update(dt);
        }
    }
    
    // Sciprt Execute
    for (const auto& script : m_vec_script){
        if(script){
            if (m_vec_entity_status[script->GetOwnerID()] == EntityStatus::kActive)
            script->Execute(dt);
        }
    }

    // Collision Check
    CollisionMgr::Collision(dt);

    // Extra Things
    // auto mouse_state = InputMgr::GetMouseState();
    // auto mouse_pos = InputMgr::GetMousePos();

    // auto camera_script = SceneMgr::GetComponentOrigin<CameraScript>(m_main_camear_id);
    // auto camera_pos = camera_script->GetMainCameraPos();

    // // current pos
    // auto window = Core::GetWindowSize();
    // auto cur_left_top = camera_pos - window/2;

    // mouse_pos += cur_left_top;
    
    // Vec2 offset{static_cast<float>(static_cast<int>(mouse_pos.x) % static_cast<int>(gird_offset)),
    //     static_cast<float>(static_cast<int>(mouse_pos.y) % static_cast<int>(gird_offset))};
    // mouse_pos -= offset;

    // // 0~40 -> 0 , 40~ 80 -> 40 ... 
    // if (mouse_state == MouseState::kLeftTap || mouse_state == MouseState::kLeftHold){

    //     bool same{false};
        
    //     auto vec_id = SceneMgr::GetEntityVector<RectObject>();
    //     for(const auto& id: vec_id){
    //         auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(id);
    //         if ( transform ) {
    //             auto pos = transform->GetPos();
    //             if (mouse_pos == pos){
    //                 same = true;
    //                 break;
    //             }
    //         }
    //     }

    //     if ( !same ){
    //         RectObject rect{Vec2(mouse_pos.x, mouse_pos.y), Vec2(gird_offset,gird_offset)};
    //         rect.SetCollider(CollisionEntityType::kWall, Vec2(gird_offset,gird_offset));
    //     }

    // }
    // else if (mouse_state == MouseState::kRightTap || mouse_state == MouseState::kRightHold){
    //     auto vec_id = SceneMgr::GetEntityVector<RectObject>();
    //     for(const auto& id: vec_id){
    //         auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(id);
    //         if ( transform ) {
    //             auto pos = transform->GetPos();
    //             if (mouse_pos == pos){
    //                 auto vec_id = SceneMgr::GetComponentsID(transform->GetOwnerID());
    //                 for (const auto& id : vec_id){
    //                     auto& comp = SceneMgr::GetComponent(id);
    //                     SceneMgr::DeleteComponent(std::move(comp));
    //                 }
    //                 std::cout << "DELETE" << std::endl;
    //                 break;
    //             }
    //         }
    //     }
    // }
}