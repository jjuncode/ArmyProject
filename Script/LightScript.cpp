#include "LightScript.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void LightScript::Execute(float dt)
{
    uint32_t light_move_speed{250};
    auto &transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();

     // 좌우 이동 (A / D)
    if (InputMgr::IsTap(sf::Keyboard::A) || InputMgr::IsHold(sf::Keyboard::A)) {
        transform.AddPos(Vec2(-dt * light_move_speed, 0));
    }

    if (InputMgr::IsTap(sf::Keyboard::D) || InputMgr::IsHold(sf::Keyboard::D)) {
        transform.AddPos(Vec2(dt * light_move_speed, 0));
    }

    // 상하 이동 (W / S)
    if (InputMgr::IsTap(sf::Keyboard::W) || InputMgr::IsHold(sf::Keyboard::W)) {
        transform.AddPos(Vec2(0, dt * light_move_speed));
    }

    if (InputMgr::IsTap(sf::Keyboard::S) || InputMgr::IsHold(sf::Keyboard::S)) {
        transform.AddPos(Vec2(0, -dt * light_move_speed));
    }

}