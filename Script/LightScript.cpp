#include "LightScript.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void LightScript::Execute(float dt)
{
    uint32_t light_move_speed{250};
    auto &transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();

     // 좌우 이동 (A / D)
    if (InputMgr::IsTap(sf::Keyboard::A) || InputMgr::IsHold(sf::Keyboard::A)) {
    }

    if (InputMgr::IsTap(sf::Keyboard::D) || InputMgr::IsHold(sf::Keyboard::D)) {
    }

    // 상하 이동 (W / S)
    if (InputMgr::IsTap(sf::Keyboard::W) || InputMgr::IsHold(sf::Keyboard::W)) {
    }

    if (InputMgr::IsTap(sf::Keyboard::S) || InputMgr::IsHold(sf::Keyboard::S)) {
    }

}