#include "RigidbodyComponent.h"
#include "TransformComponent.h"

#include "../Mgr/SceneMgr.h"


void Rigidbody::Update(float dt)
{
    auto transform = SceneMgr::GetComponent<TransformComponent>(GetOwnerID());
	auto pos = transform->GetPos();

	// Apply gravity
	Vec2 force_gravity = m_gravity * m_mass;
	m_force += force_gravity;

	// a = f / m;
	m_accel = m_force / m_mass;

	// v = v0 + a * t
	m_velocity += m_accel * dt;

	// 최대 속도 제한
	if (Vec::Length(m_velocity) > Vec::Length(m_velocity_max)) {
		m_velocity = Vec::Normalize(m_velocity) * Vec::Length(m_velocity_max);
	}

	// on ground 
	if ( pos.y <= 0 ){
		m_velocity.x *= m_fric;
		// if (Vec::Length(m_velocity) <= 100.f){
		// 	// stop
		// 	m_velocity = Vec2(0, 0);
		// }

		transform->SetPos(Vec2(pos.x, 0));
		if ( m_velocity.y < 0 )
			m_velocity.y = 0;
	}

	transform->AddPos(m_velocity * dt);
	m_force = Vec2(0,0);

} 

void ProcessImpulseColl(uint32_t self_entity_id, uint32_t other_entity_id, MTV _mtv, float dt)
{
    auto transform_other = SceneMgr::GetComponent<TransformComponent>(other_entity_id);
    auto rigidbody_other = SceneMgr::GetComponent<Rigidbody>(other_entity_id);
  
    auto transform_self = SceneMgr::GetComponent<TransformComponent>(self_entity_id);
	auto rigidbody_self = SceneMgr::GetComponent<Rigidbody>(self_entity_id);

	Vec2 relative_velo = rigidbody_self->GetVelocity() - rigidbody_other->GetVelocity();
	float elastic =  (rigidbody_self->GetElastic() * rigidbody_other->GetElastic() )/ 2 ;

	// 충돌 방향으로의 속도 성분
	float velo_extract = Vec::Dot(relative_velo, _mtv.vec);

    // Get Impulse
	float j = -(1 + elastic) * velo_extract / (1.0f / rigidbody_self->GetMass() + 1.0f / rigidbody_other->GetMass());
    Vec2 impulse = _mtv.vec * j;

	// apply impulse
    rigidbody_self->ApplyImpulse( impulse / rigidbody_self->GetMass());
	rigidbody_other->ApplyImpulse(Vec::Reverse(impulse) / rigidbody_other->GetMass());
 
    transform_other->AddPos(_mtv.vec * _mtv.length);
}