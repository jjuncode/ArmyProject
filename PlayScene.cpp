#include "PlayScene.h"
#include "pch.h"
#include "Core.h"
#include "mymath.h"

#include "Mgr/InputMgr.h"
#include "Mgr/SceneMgr.h"
#include "Mgr/CollisionMgr.h"

#include "Component/RigidbodyComponent.h"

#include "Script/CameraScript.h"
#include "Script/PlayerScript.h"
#include "Script/BoxScript.h"
#include "Script/LightScript.h"

void PlayScene::Init()
{
    Vec3 size = Vec3(50,50,50);

    auto player = Object::CreateObject(Vec3(0,0,-300), Vec3(size));
  //  player->GetTransform().AddRotate(Vec3(45.f,45.f,45.f));
    // player.SetCollider(CollisionEntityType::kPlayer, Vec2(size,size));
    // player.AddComponent<Rigidbody>(25.f,0.9f);
    auto script = Script::CreateScript<PlayerScript>();
    player->SetScript(std::move(script));
    player->SetMesh("square");
    player->SetTexutre("player.png");
    player->SetColor(sf::Color::Yellow);
    
    auto camera = Object::CreateObject();
    auto script_camera = Script::CreateScript<CameraScript>(player->GetObjectID());
    camera->SetVisible(false);
    camera->SetScript(std::move(script_camera));
    camera->GetTransform().SetLocalAxis(Vec3(0, 0, -1), Vec3(0, 1, 0), Vec3(-1, 0, 0));

    auto plane = Object::CreateObject(Vec3(0,0,-100), Vec3(size*2));
    plane->SetMesh("plane");
    plane->SetColor(sf::Color::Blue);

    for (int i=0; i<100; ++i){
        auto pos = Vec3(GetRandomInt(0, 500), GetRandomInt(0,500), GetRandomInt(-1000, 0));
        auto obj = Object::CreateObject(pos, size);
        obj->SetMesh("square");
        obj->SetTexutre("player.png");
        obj->SetColor(sf::Color::Blue);
        SceneMgr::AddObject(std::move(obj));
    }

    // auto light = Object::CreateObject(Vec3(0,150,0),size);
    // auto script_light = Script::CreateScript<LightScript>(sf::Color::Red);
    // script_light->SetRange(400.f);
    // light->SetScript(std::move(script_light));
    // light->SetVisible(true);
    // light->SetMesh("circle");
    // light->SetColor(sf::Color::Red);
    // light->SetShading(false);
    // SceneMgr::AddLight(std::move(light));
    
    SceneMgr::SetMainCamera(camera);
    SceneMgr::AddObject(std::move(player));
    SceneMgr::AddObject(std::move(camera));
    SceneMgr::AddObject(std::move(plane));

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
        if ( obj ) 
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