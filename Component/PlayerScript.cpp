#include "PlayerScript.h"

#include "TransformComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void PlayerScript::Update(float dt)
{
    int grid_offset{40};

    uint32_t player_speed{500};
    uint32_t player_rotate_speed{300};

    if ( InputMgr::IsTap(sf::Keyboard::Left) || InputMgr::IsHold(sf::Keyboard::Left)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->AddPos(Vec2(-dt*player_speed,0));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Right) || InputMgr::IsHold(sf::Keyboard::Right)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->AddPos(Vec2(dt*player_speed,0));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Up) || InputMgr::IsHold(sf::Keyboard::Up)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->AddPos(Vec2(0,-dt*player_speed));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Down) || InputMgr::IsHold(sf::Keyboard::Down)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->AddPos(Vec2(0,dt*player_speed));
    }
}