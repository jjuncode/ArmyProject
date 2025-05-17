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
#include "Script/BoxScript.h"

void PlayScene::Init()
{
    // GridObject grid;
    // grid.Init(gird_offset);

    int size = 100;

    PlayerObject player{Vec2(50,500), Vec2(size,size)};
    player.SetCollider(CollisionEntityType::kPlayer, Vec2(size,size));
    player.AddComponent<Rigidbody>(25.f,0.9f);
    player.SetScript<PlayerScript>();

    Camera player_camera{player.GetEntityID()};
    player_camera.SetScript<CameraScript>(player.GetEntityID());
    player_camera.SetMainCamera();

    Polygon poly3{1,Vec2(0,0), Vec2()};
    poly3.SetCollider(CollisionEntityType::kBox, Vec2(20000,10));
    poly3.SetScript<BoxScript>();
    poly3.AddComponent<Rigidbody>(true);

    SetCollisionLayer(CollisionEntityType::kPlayer, CollisionEntityType::kBox, true);
    SetCollisionLayer(CollisionEntityType::kBox, CollisionEntityType::kBox, true);
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
    auto mouse_state = InputMgr::GetMouseState();
    auto mouse_pos = InputMgr::GetMousePos();

    auto camera_script = SceneMgr::GetScript<CameraScript>(m_main_camear_id);
    auto camera_pos = camera_script->GetMainCameraPos();
    auto zoom_value = camera_script->GetZoomValue();

    // // current pos
    auto window = Core::GetWindowSize();
    camera_pos += Vec2(-window.x , window.y)/2 * (1/zoom_value);
    mouse_pos *= 1/zoom_value;
    mouse_pos.y *= -1;
    mouse_pos += camera_pos;

    static uint32_t lastest_obj_id{};

    if (mouse_state == MouseState::kLeftTap){
        int size = 100;
        Polygon poly{4, Vec2(mouse_pos.x, mouse_pos.y), Vec2(size, size)};
        poly.SetCollider(CollisionEntityType::kBox, Vec2(size, size));
        poly.SetScript<BoxScript>();
        poly.AddComponent<Rigidbody>(25.f, 0.9f);

        lastest_obj_id = poly.GetEntityID();
    }

    if ( InputMgr::IsTap(sf::Keyboard::Key::BackSpace)){
        DeleteEntity(lastest_obj_id);
        lastest_obj_id -=1;
    }
}