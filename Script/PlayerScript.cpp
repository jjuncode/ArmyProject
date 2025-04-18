#include "PlayerScript.h"

#include "../Component/TransformComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void PlayerScript::Execute(float dt)
{
    int grid_offset{40};

    uint32_t player_speed{500};
    uint32_t player_rotate_speed{10};

    auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
    auto pos = transform->GetPos();

    if ( InputMgr::IsTap(sf::Keyboard::Left) || InputMgr::IsHold(sf::Keyboard::Left)){
        transform->AddPos(Vec2(-dt*player_speed,0));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Right) || InputMgr::IsHold(sf::Keyboard::Right)){
        transform->AddPos(Vec2(dt*player_speed,0));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Up) || InputMgr::IsHold(sf::Keyboard::Up)){
        transform->AddPos(Vec2(0,dt*player_speed));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Down) || InputMgr::IsHold(sf::Keyboard::Down)){
        transform->AddPos(Vec2(0,-dt*player_speed));
    }

    if (InputMgr::IsTap(sf::Keyboard::R) || InputMgr::IsHold(sf::Keyboard::R)){
        transform->AddRotate(dt*player_rotate_speed);
    }
}

void PlayerScript::ExecuteCollEnter(uint32_t other_entity_id, float dt)
{
}

void PlayerScript::ExecuteCollStay(uint32_t other_entity_id, float dt)
{
}

void PlayerScript::ExecuteCollExit(uint32_t other_entity_id, float dt)
{
}
