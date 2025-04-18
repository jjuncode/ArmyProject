#include "PhysicComponent.h"
#include "TransformComponent.h"

#include "../Mgr/SceneMgr.h"
#include "../Core.h"

int PhysicComponent::m_gravity{30};
float PhysicComponent::m_air_friction{0.8};

void PhysicComponent::Update(float dt)
{
    auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
    
    // Gravity
    m_accel.y -= m_gravity;

    // Air Friction
    m_accel *= m_air_friction;

    auto window = Core::GetWindowSize();
    
    if ( transform->GetPos().y +transform->GetScale().y/2 >= window.y ){
        transform->SetPos({transform->GetPos().x, window.y - transform->GetScale().y/2});
        m_velocity.y = 0;
    }

    m_velocity += m_accel*dt;

    transform->AddPos(m_velocity*dt);
}