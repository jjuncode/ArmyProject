#include "PlayerScript.h"

#include "TransformComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void PlayerScript::Update(float dt)
{
    int grid_offset{40};

    if ( InputMgr::IsTap(sf::Keyboard::Left)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->SetPos(pos-Vec2(grid_offset,0));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Right)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->SetPos(pos+Vec2(grid_offset,0));
    }

    if ( InputMgr::IsTap(sf::Keyboard::Up)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->SetPos(pos-Vec2(0,grid_offset));

        std::cout << "POS : " << transform->GetPos().x << ", " << transform->GetPos().y << std::endl;
    }

    if ( InputMgr::IsTap(sf::Keyboard::Down)){
        auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
        auto pos = transform->GetPos();
        transform->SetPos(pos+Vec2(0,grid_offset));
    }
}