#include "ObjectScript.h"

#include "../Component/AnimationComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void ObjectScript::Execute(float dt)
{
    int grid_offset{40};

    uint32_t player_speed{50};
    uint32_t player_rotate_speed{10};

    auto &transform = SceneMgr::GetObject(GetOwnerID()).GetTransform();
    transform.AddLocalRotate(Vec3(0,1,0) * dt * player_rotate_speed);
    
    // 방향키 입력 처리 (Tap 또는 Hold)
    if (InputMgr::IsTap(sf::Keyboard::Left) || InputMgr::IsHold(sf::Keyboard::Left)) {
        transform.AddLocalPosition(Vec3(-1, 0, 0) * dt * player_speed);
        std::cout << "Left Key Pressed" << std::endl;
    }
    if (InputMgr::IsTap(sf::Keyboard::Right) || InputMgr::IsHold(sf::Keyboard::Right)) {
        transform.AddLocalPosition(Vec3(1, 0, 0) * dt * player_speed);
        std::cout << "Right Key Pressed" << std::endl;
    }
    if (InputMgr::IsTap(sf::Keyboard::Up) || InputMgr::IsHold(sf::Keyboard::Up)) {
        transform.AddLocalPosition(Vec3(0, 1, 0) * dt * player_speed);
    }
    if (InputMgr::IsTap(sf::Keyboard::Down) || InputMgr::IsHold(sf::Keyboard::Down)) {
        transform.AddLocalPosition(Vec3(0, -1, 0) * dt * player_speed);
    }
}

void ObjectScript::ExecuteCollEnter(uint32_t other_entity_id, MTV _mtv,float dt)
{
}

void ObjectScript::ExecuteCollStay(uint32_t other_entity_id, MTV _mtv,float dt)
{
}

void ObjectScript::ExecuteCollExit(uint32_t other_entity_id,MTV _mtv, float dt)
{
}
