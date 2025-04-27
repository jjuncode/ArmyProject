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

	// Apply Friction
	auto fric = (rigidbody_self->GetFric() + rigidbody_other->GetFric()) /2 ;
	rigidbody_self->SetVelocity(rigidbody_self->GetVelocity() *fric);
	rigidbody_other->SetVelocity(rigidbody_other->GetVelocity() *fric);

	// Get Elasticity
	Vec2 relative_velo = rigidbody_self->GetVelocity() - rigidbody_other->GetVelocity();
	float elastic =  (rigidbody_self->GetElastic() + rigidbody_other->GetElastic() )/ 2 ;

	// 충돌 방향으로의 속도 성분
	float velo_extract = Vec::Dot(relative_velo, _mtv.vec);

    // Get Impulse
	float j = -(1 + elastic) * velo_extract / ((1.0f / rigidbody_self->GetMass()) + (1.0f / rigidbody_other->GetMass()));
    Vec2 impulse = _mtv.vec * j;

	// Apply impulse
    rigidbody_self->ApplyImpulse( impulse / rigidbody_self->GetMass());
	rigidbody_other->ApplyImpulse(Vec::Reverse(impulse) / rigidbody_other->GetMass());
 
	// Compare Mass
	auto mass_self = rigidbody_self->GetMass();
	auto mass_other = rigidbody_other->GetMass();

	// Set Move Ratio
	float move_ratio{mass_self / (mass_self + mass_other)};

	// Set MTV direction
	Vec2 direction{};
	auto pos_subtraction = transform_self->GetPos() - transform_other->GetPos();
	if ( Vec::Dot(pos_subtraction, _mtv.vec) < 0){
		direction = Vec::Reverse(_mtv.vec);
	}
	else{
		direction = _mtv.vec;
	}

	transform_self->AddPos( direction * _mtv.length * move_ratio);
    transform_other->AddPos( Vec::Reverse(direction) * _mtv.length * ( 1- move_ratio));
}