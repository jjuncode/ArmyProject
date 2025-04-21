#include "WallScript.h"

#include "../Component/TransformComponent.h"
#include "../Component/ColliderComponent.h"
#include "../Component/RigidbodyComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void WallScript::Execute(float dt)
{
}

void WallScript::ExecuteCollEnter(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    auto rigidbody = SceneMgr::GetComponent<Rigidbody>(other_entity_id);

    coll->SetOBBColor(sf::Color::Red);
    
    // Reflection
    auto direction = _mtv.vec;
    auto velo = rigidbody->GetVelocity();
    auto force = direction * Vec::Reverse(velo) * 2 ;
    
    rigidbody->SetVelocity(Vec2(0,0));
    rigidbody->AddForce(force);


    std::cout <<"나 강림 ㅇㅇ " << std::endl;
}

void WallScript::ExecuteCollStay(uint32_t other_entity_id,MTV _mtv, float dt)
{
}

void WallScript::ExecuteCollExit(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Green);
}
