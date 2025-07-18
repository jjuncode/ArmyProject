#include "BoxScript.h"

#include "../Component/ColliderComponent.h"
#include "../Component/RigidbodyComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void BoxScript::Execute(float dt)
{
}

void BoxScript::ExecuteCollEnter(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Red);
    
    Physic::ProcessPhysicCollision(GetOwnerID(), other_entity_id, _mtv, dt);
}

void BoxScript::ExecuteCollStay(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Red);
    
    Physic::ProcessPhysicCollision(GetOwnerID(), other_entity_id, _mtv, dt);
}

void BoxScript::ExecuteCollExit(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Green);
}
