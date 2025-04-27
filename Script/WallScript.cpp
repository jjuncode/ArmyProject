#include "WallScript.h"

#include "../Component/TransformComponent.h"
#include "../Component/ColliderComponent.h"
#include "../Component/RigidbodyComponent.h"

#include "../Mgr/InputMgr.h"
#include "../Mgr/SceneMgr.h"

void WallScript::Execute(float dt)
{
    if ( InputMgr::IsTap(sf::Keyboard::T) || InputMgr::IsHold(sf::Keyboard::T) ){
        auto transform = SceneMgr::GetComponent<TransformComponent>(GetOwnerID());
        transform->AddRotate(dt*100);
    }   
}

void WallScript::ExecuteCollEnter(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Red);
    
    ProcessImpulseColl(GetOwnerID(), other_entity_id, _mtv, dt);
    
    // auto transform_other = SceneMgr::GetComponent<TransformComponent>(other_entity_id);
    // auto rigidbody_other = SceneMgr::GetComponent<Rigidbody>(other_entity_id);
    // auto rigidbody_self = SceneMgr::GetComponent<Rigidbody>(GetOwnerID());
    
    // // Reflection
    // auto direction = _mtv.vec;
    // auto velo = rigidbody_other->GetVelocity();

    // float fric_other = rigidbody_other->GetFric();
    // float fric_self = rigidbody_self->GetFric();
    // float fric = (fric_other + fric_self) / 2;
   
    // auto force = direction * Vec::Length(velo) * fric;

    // // collision by upside
    // if ( velo.y < 0 && _mtv.vec.y >0 ){
    //     rigidbody_other->SetVelocity(Vec2(velo.x, 0));
    // }

    // rigidbody_other->ApplyImpulse(force);

    // transform_other->AddPos(_mtv.vec * _mtv.length);
}

void WallScript::ExecuteCollStay(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Red);
    
    ProcessImpulseColl(GetOwnerID(), other_entity_id, _mtv, dt);
    // auto transform_other = SceneMgr::GetComponent<TransformComponent>(other_entity_id);
    // auto rigidbody_other = SceneMgr::GetComponent<Rigidbody>(other_entity_id);
    // auto rigidbody_self = SceneMgr::GetComponent<Rigidbody>(GetOwnerID());

    // auto velo = rigidbody_other->GetVelocity();

    // // collision by upside
    // if ( velo.y < 0 && _mtv.vec.y > 0 ){
    //     rigidbody_other->SetVelocity(Vec2(velo.x,0));
    // }
    
    // transform_other->AddPos(_mtv.vec * _mtv.length);
}

void WallScript::ExecuteCollExit(uint32_t other_entity_id,MTV _mtv, float dt)
{
    auto coll = SceneMgr::GetComponent<ColliderComponent>(GetOwnerID());
    coll->SetOBBColor(sf::Color::Green);
}
