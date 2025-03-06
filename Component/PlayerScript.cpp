#include "PlayerScript.h"

#include "TransformComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void PlayerScript::Update(float dt)
{
    if ( InputMgr::IsTap(sf::Keyboard::Left)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->SetPos(pos-Vec2(40,0));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Right)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->SetPos(pos+Vec2(40,0));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Up)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->SetPos(pos+Vec2(0,-40));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Down)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->SetPos(pos+Vec2(0,40));
    }
}