#include "PlayerScript.h"

#include "../Component/TransformComponent.h"
#include "../Component/ColliderComponent.h"
#include "../Component/RigidbodyComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void PlayerScript::Execute(float dt)
{
    int grid_offset{40};

    uint32_t player_speed{500};
    uint32_t player_rotate_speed{1};

    auto transform = SceneMgr::GetComponent<TransformComponent>(GetOwnerID());
    auto pos = transform->GetPos();

    if ( InputMgr::IsTap(sf::Keyboard::Left) || InputMgr::IsHold(sf::Keyboard::Left)){
        auto rigidbody = SceneMgr::GetComponent<Rigidbody>(GetOwnerID());
        rigidbody->ApplyImpulse(Vec2(-dt*player_speed,0));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Right) || InputMgr::IsHold(sf::Keyboard::Right)){
        auto rigidbody = SceneMgr::GetComponent<Rigidbody>(GetOwnerID());
        rigidbody->ApplyImpulse(Vec2(dt*player_speed,0));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Up) || InputMgr::IsHold(sf::Keyboard::Up)){
        transform->AddPos(Vec2(0,dt*player_speed));
        auto rigidbody = SceneMgr::GetComponent<Rigidbody>(GetOwnerID());
        auto gravity = rigidbody->GetGravity();

        auto velo = rigidbody->GetVelocity();
        rigidbody->SetVelocity(Vec2(velo.x, 0));
    }

    if (InputMgr::IsTap(sf::Keyboard::R) || InputMgr::IsHold(sf::Keyboard::R)){
        transform->AddRotate(dt*player_rotate_speed);
    }

    if ( InputMgr::IsTap(sf::Keyboard::Space)){
        transform->AddPos(Vec2(0,1));
        auto rigidbody = SceneMgr::GetComponent<Rigidbody>(GetOwnerID());

        rigidbody->ApplyImpulse (Vec::Reverse(rigidbody->GetGravity()) ) ;
        rigidbody->ApplyImpulse(Vec2(0, 100));
        std::cout<<"JUMP"<<std::endl;
    }
}

void PlayerScript::ExecuteCollEnter(uint32_t other_entity_id, MTV _mtv,float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Red);
}

void PlayerScript::ExecuteCollStay(uint32_t other_entity_id, MTV _mtv,float dt)
{
}

void PlayerScript::ExecuteCollExit(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Green);
}
