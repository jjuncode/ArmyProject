#include "PlayerScript.h"

#include "../Component/ColliderComponent.h"
#include "../Component/RigidbodyComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void PlayerScript::Execute(float dt)
{
    int grid_offset{40};

    uint32_t player_speed{500};
    uint32_t player_jump{10};
    uint32_t player_rotate_speed{50};

    auto &transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();
    // auto rigidbody = SceneMgr::GetComponent<Rigidbody>(GetOwnerID());
    auto pos = transform.GetPos();

    if ( InputMgr::IsTap(sf::Keyboard::Left) || InputMgr::IsHold(sf::Keyboard::Left)){
    }

    if (InputMgr::IsTap(sf::Keyboard::Right) || InputMgr::IsHold(sf::Keyboard::Right)){
    }

    if (InputMgr::IsTap(sf::Keyboard::Up) || InputMgr::IsHold(sf::Keyboard::Up)){
    }

    if (InputMgr::IsTap(sf::Keyboard::Down) || InputMgr::IsHold(sf::Keyboard::Down)) {
    }
    float speed = dt * player_rotate_speed;

    if (InputMgr::IsTap(sf::Keyboard::Z) || InputMgr::IsHold(sf::Keyboard::Z)) {
        transform.AddRotate(Vec3(0, 0, 1) * speed); // Z축 회전 (Roll)
    }
    if (InputMgr::IsTap(sf::Keyboard::X) || InputMgr::IsHold(sf::Keyboard::X)) {
        transform.AddRotate(Vec3(1, 0, 0) * speed); // X축 회전 (Pitch)
    }
    if (InputMgr::IsTap(sf::Keyboard::C) || InputMgr::IsHold(sf::Keyboard::C)) {
        transform.AddRotate(Vec3(0, 1, 0) * speed); // Y축 회전 (Yaw)
    }

    if (InputMgr::IsTap(sf::Keyboard::O)) {
        transform.AddRotate(Vec3(0, 90, 0)); // Y축 회전 (Yaw)
    }

    if ( InputMgr::IsTap(sf::Keyboard::T)){
        static bool toogle{false};
        SceneMgr::GetObject(GetOwnerID()).SetVisible(toogle);

        toogle = !toogle;
    }
}

void PlayerScript::ExecuteCollEnter(uint32_t other_entity_id, MTV _mtv,float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Red);

    Physic::ProcessPhysicCollision(GetOwnerID(), other_entity_id, _mtv, dt);
}

void PlayerScript::ExecuteCollStay(uint32_t other_entity_id, MTV _mtv,float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Red);

    Physic::ProcessPhysicCollision(GetOwnerID(), other_entity_id, _mtv, dt);
}

void PlayerScript::ExecuteCollExit(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Green);
}
