#include "RigidbodyComponent.h"
#include "TransformComponent.h"

#include "../Mgr/SceneMgr.h"


void Rigidbody::Update(float dt)
{
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
	
	// 적용된 힘이 없고 속도가 0이 아닐 떄(마찰력의 조건)
	if (!(m_velocity == Vec2(0,0)))
	{
		//속도에 반대방향으로 마찰력 작용
		Vec2 direction_fric = Vec::Normalize(Vec::Reverse(m_velocity));
		Vec2 friction = direction_fric * m_fric * m_mass * dt;
		// 마찰력으로 인한 속도 감소량이 현재 속도보다 큰 경우
		if ( Vec::Length(m_velocity) <= Vec::Length(friction))
		{
			// 멈춰
			m_velocity = Vec2(0,0);
		}
		else
		{
			// 속도에서 마찰력으로 인한 반대방향으로 속도를 차감
			m_velocity += friction;
		}
	}
	
	//계산된 속도에 맞게 위치를 조정
    auto transform = SceneMgr::GetComponent<TransformComponent>(GetOwnerID());
	
	if ( transform->GetPos().y <= 0 ){
		transform->SetPos(Vec2(transform->GetPos().x, 0));
		if ( m_velocity.y < 0 )
			m_velocity.y = 0;
	}

	transform->AddPos(m_velocity * dt);
	m_force = Vec2(0,0);

}