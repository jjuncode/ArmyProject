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
    auto transform_other = SceneMgr::GetComponent<TransformComponent>(other_entity_id);
    auto rigidbody_other = SceneMgr::GetComponent<Rigidbody>(other_entity_id);
    auto rigidbody_self = SceneMgr::GetComponent<Rigidbody>(GetOwnerID());

    coll->SetOBBColor(sf::Color::Red);
    
    // Reflection
    auto direction = _mtv.vec;
    auto velo = rigidbody_other->GetVelocity();

    float fric_other = rigidbody_other->GetFric();
    float fric_self = rigidbody_self->GetFric();
    float fric = (fric_other + fric_self) / 2;
   
    auto force = direction * Vec::Length(velo) * fric;

     // side force is little bit less than up force
     if ( direction.x != 0 ){
        force.x *= 0.1f;
    }

    // collision by upside
    if ( velo.y < 0 && _mtv.vec.y >0 ){
        rigidbody_other->SetVelocity(Vec2(velo.x, 0));
    }

    rigidbody_other->ApplyImpulse(force);

    transform_other->AddPos(_mtv.vec * _mtv.length);
}

void WallScript::ExecuteCollStay(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto transform_other = SceneMgr::GetComponent<TransformComponent>(other_entity_id);
    auto rigidbody_other = SceneMgr::GetComponent<Rigidbody>(other_entity_id);
    auto rigidbody_self = SceneMgr::GetComponent<Rigidbody>(GetOwnerID());

    auto velo = rigidbody_other->GetVelocity();

    // collision by upside
    if ( velo.y < 0 && _mtv.vec.y > 0 ){
        rigidbody_other->SetVelocity(Vec2(velo.x,0));
    }
    
    transform_other->AddPos(_mtv.vec * _mtv.length);
}

void WallScript::ExecuteCollExit(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Green);
}
