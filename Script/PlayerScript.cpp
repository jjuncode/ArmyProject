#include "PlayerScript.h"

#include "../Component/ColliderComponent.h"
#include "../Component/RigidbodyComponent.h"
#include "../Component/AnimationComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void PlayerScript::Execute(float dt)
{
    int grid_offset{40};

    uint32_t player_speed{10};
    uint32_t player_jump{10};
    uint32_t player_rotate_speed{50};

    auto &transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();
    transform.AddLocalRotate(Vec3(0,1,0) * dt * player_rotate_speed);
    // auto rigidbody = SceneMgr::GetComponent<Rigidbody>(GetOwnerID());
    auto pos = transform.GetPos();

    if ( InputMgr::IsTap(sf::Keyboard::Left) || InputMgr::IsHold(sf::Keyboard::Left)){
        transform.AddLocalPosition(Vec3(-1, 0, 0) * dt * player_speed);
    }

    if (InputMgr::IsTap(sf::Keyboard::Right) || InputMgr::IsHold(sf::Keyboard::Right)){
        transform.AddLocalPosition(Vec3(1, 0, 0) * dt * player_speed);
    }

    if (InputMgr::IsTap(sf::Keyboard::Up) || InputMgr::IsHold(sf::Keyboard::Up)){
        transform.AddLocalPosition(Vec3(0, 1, 0) * dt * player_speed);
    }

    if (InputMgr::IsTap(sf::Keyboard::Down) || InputMgr::IsHold(sf::Keyboard::Down)) {
        transform.AddLocalPosition(Vec3(0, -1, 0) * dt * player_speed);
    }

    if (InputMgr::IsTap(sf::Keyboard::I) || InputMgr::IsHold(sf::Keyboard::I)) {
        transform.AddPos(Vec3(0, 1, 0) * dt * player_speed);
    }

    if (InputMgr::IsTap(sf::Keyboard::J) || InputMgr::IsHold(sf::Keyboard::J)) {
        transform.AddPos(Vec3(-1, 0, 0) * dt * player_speed);
    }

    if (InputMgr::IsTap(sf::Keyboard::K) || InputMgr::IsHold(sf::Keyboard::K)) {
        transform.AddPos(Vec3(0, -1, 0) * dt * player_speed);
    }

    if (InputMgr::IsTap(sf::Keyboard::L) || InputMgr::IsHold(sf::Keyboard::L)) {
        transform.AddPos(Vec3(1, 0, 0) * dt * player_speed);
    }

    float speed = dt * player_rotate_speed;

    if (InputMgr::IsTap(sf::Keyboard::Z) || InputMgr::IsHold(sf::Keyboard::Z)) {
        transform.AddLocalRotate(Vec3(0, 0, 1) * speed); // Z축 회전 (Roll)
    }
    if (InputMgr::IsTap(sf::Keyboard::X) || InputMgr::IsHold(sf::Keyboard::X)) {
        transform.AddLocalRotate(Vec3(1, 0, 0) * speed); // X축 회전 (Pitch)
    }
    if (InputMgr::IsTap(sf::Keyboard::C) || InputMgr::IsHold(sf::Keyboard::C)) {
        transform.AddLocalRotate(Vec3(0, 1, 0) * speed); // Y축 회전 (Yaw)
    }

    if (InputMgr::IsTap(sf::Keyboard::O) || InputMgr::IsHold(sf::Keyboard::O)) {
        auto comp_ani = SceneMgr::GetObject(GetOwnerID()).GetAnimation();
        if (comp_ani) {
            comp_ani->GetBone("left").GetCurTransform().AddPos(Vec3(dt,0,0));
        }
    }

    if (InputMgr::IsTap(sf::Keyboard::P) || InputMgr::IsHold(sf::Keyboard::P)) {
        auto comp_ani = SceneMgr::GetObject(GetOwnerID()).GetAnimation();
        if (comp_ani) {
            comp_ani->GetBone("right").GetCurTransform().AddPos(Vec3(-dt,0,0));
        }
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
