#include "PhysicComponent.h"
#include "TransformComponent.h"

#include "../Mgr/SceneMgr.h"

int PhysicComponent::m_gravity{10};
float PhysicComponent::m_air_friction{0.8};

void PhysicComponent::Update(float dt)
{
    auto transform = SceneMgr::GetComponentOrigin<TransformComponent>(GetOwnerID());
    
    // Gravity
    m_accel.y -= m_gravity;

    // Air Friction
    m_accel *= m_air_friction;

    m_velocity += m_accel*dt;
}