#include "PlayScene.h"
#include "pch.h"
#include "Core.h"

#include "Object/Camera.h"

#include "Mgr/InputMgr.h"
#include "Mgr/SceneMgr.h"
#include "Mgr/CollisionMgr.h"

#include "Component/RigidbodyComponent.h"
#include "Component/Mesh.h"
#include "Component/Texture.h"

#include "Script/CameraScript.h"
#include "Script/PlayerScript.h"
#include "Script/BoxScript.h"

void PlayScene::Init()
{
    int size = 200;

    auto player = Object::CreateObject(Vec2(0,0), Vec2(size,size));
    // player.SetCollider(CollisionEntityType::kPlayer, Vec2(size,size));
    // player.AddComponent<Rigidbody>(25.f,0.9f);
    auto script = Script::CreateScript<PlayerScript>();
    player->SetScript(std::move(script));
    player->SetMesh("square");
    player->SetTexutre("player.png");
    SceneMgr::AddObject(std::move(player));
    

    // Camera player_camera{player.GetEntityID()};
    // player_camera.SetScript<CameraScript>(player.GetEntityID());
    // player_camera.SetMainCamera();

    SetCollisionLayer(CollisionObjectType::kPlayer, CollisionObjectType::kBox, true);
    SetCollisionLayer(CollisionObjectType::kBox, CollisionObjectType::kBox, true);
}

void PlayScene::Update(float dt)
{
    // Entity Update
    for (const auto &comp : m_vec_component){
        if (comp){
            if (IsActiveObject(comp->GetOwnerID()))
                comp->Update(dt);
        }
    }
    
    // Sciprt Execute
    for (const auto& obj : m_vec_object ){
        if (IsActiveObject(obj->GetObjectID()))
            obj->Execute(dt);
    }

    // Collision Check
    CollisionMgr::Collision(dt);

    // Extra Things
    // auto mouse_state = InputMgr::GetMouseState();
    // auto mouse_pos = InputMgr::GetMousePos();

    // auto camera_script = SceneMgr::GetScript<CameraScript>(m_main_camear_id);
    // auto camera_pos = camera_script->GetMainCameraPos();
    // auto zoom_value = camera_script->GetZoomValue();

    // // // current pos
    // auto window = Core::GetWindowSize();
    // camera_pos += Vec2(-window.x , window.y)/2 * (1/zoom_value);
    // mouse_pos *= 1/zoom_value;
    // mouse_pos.y *= -1;
    // mouse_pos += camera_pos;

    // static uint32_t lastest_obj_id{};

    // if (mouse_state == MouseState::kLeftTap){
    //     int size = 100;
    //     Polygon poly{4, Vec2(mouse_pos.x, mouse_pos.y), Vec2(size, size)};
    //     poly.SetCollider(CollisionEntityType::kBox, Vec2(size, size));
    //     poly.SetScript<BoxScript>();
    //     poly.AddComponent<Rigidbody>(25.f, 0.9f);

    //     lastest_obj_id = poly.GetEntityID();
    // }

    // if ( InputMgr::IsTap(sf::Keyboard::Key::BackSpace)){
    //     DeleteEntity(lastest_obj_id);
    //     lastest_obj_id -=1;
    // }
}