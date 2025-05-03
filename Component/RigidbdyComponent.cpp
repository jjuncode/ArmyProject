#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

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

	// Apply Angular
	transform->AddRotate(m_velo_angular * dt);

	float angular_damp = 0.2f;	// 감쇠 비율 20%
	m_velo_angular *= (1 - angular_damp);

	// 최대 속도 제한
	if (Vec::Length(m_velocity) > Vec::Length(m_velocity_max)) {
		m_velocity = Vec::Normalize(m_velocity) * Vec::Length(m_velocity_max);
	}

	// on ground 
	if ( pos.y <= 0 ){
		m_velocity.x *= m_fric;
		m_velo_angular *= 0.5f;

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

	if (rigidbody_other && rigidbody_self){
		// Set Move Ratio
		// Compare Mass
		auto mass_self = rigidbody_self->GetMass();
		auto mass_other = rigidbody_other->GetMass();
		float move_ratio{mass_self / (mass_self + mass_other)};

		// Set MTV direction
		Vec2 direction{};
		auto pos_subtraction = transform_self->GetPos() - transform_other->GetPos();
		if (Vec::Dot(pos_subtraction, _mtv.vec) < 0){
			direction = Vec::Reverse(_mtv.vec);
		}
		else{
			direction = _mtv.vec;
		}

		transform_self->AddPos(direction * _mtv.length * move_ratio);
		transform_other->AddPos(Vec::Reverse(direction) * _mtv.length * (1 - move_ratio));

		// Get Elasticity
		Vec2 relative_velo = rigidbody_self->GetVelocity() - rigidbody_other->GetVelocity();
		float elastic = (rigidbody_self->GetElastic() + rigidbody_other->GetElastic()) / 2;

		// 충돌 방향으로의 속도 성분
		float velo_extract = Vec::Dot(relative_velo, _mtv.vec);

		// Apply Friction
		float fric = (rigidbody_self->GetFric() + rigidbody_other->GetFric()) / 2;
		rigidbody_self->SetVelocity(rigidbody_self->GetVelocity() * fric);
		rigidbody_other->SetVelocity(rigidbody_other->GetVelocity() * fric);

		// Get Impulse
		float j = -(1 + elastic) * velo_extract / ((1.0f / rigidbody_self->GetMass()) + (1.0f / rigidbody_other->GetMass()));
		Vec2 impulse = _mtv.vec * j;

		// Apply impulse
		rigidbody_self->ApplyImpulse(impulse);
		rigidbody_other->ApplyImpulse(Vec::Reverse(impulse));

		rigidbody_self->SetAngularVelocity(rigidbody_self->GetAngularVelocity() * fric);
		rigidbody_other->SetAngularVelocity(rigidbody_other->GetAngularVelocity() * fric);

		// Get Contact point
		auto contact_info = GetCollisionPart(self_entity_id, other_entity_id);

		auto pos_self = transform_self->GetPos();
		auto pos_other = transform_other->GetPos();
		auto pos_center = (pos_self + pos_other) / 2;

		Vec2 angular_direction_self{};
		Vec2 angular_direction_other{};

		if (!contact_info.coll_by_side){
			// coll by Dot 
			angular_direction_self = contact_info.contact_point_self - pos_self; 	// Dot 충돌이므로 다 self로 해도 상관없다.
			angular_direction_other = contact_info.contact_point_self - pos_other; // Dot 충돌이므로 다 self로 해도 상관없다.
		}
		else{
			// coll by side
			auto distn_self = Vec::LengthSquare(contact_info.contact_point_self - pos_center);
			auto distn_other = Vec::LengthSquare(contact_info.contact_point_other - pos_center);

			Vec2 torque_dot{};
			if ( distn_self < distn_other){
				torque_dot = contact_info.contact_point_self;
			}
			else if ( distn_self > distn_other){
				torque_dot = contact_info.contact_point_other;
			}
			else{
				torque_dot = (contact_info.contact_point_self + contact_info.contact_point_other )/ 2 ;
			}

			angular_direction_self = torque_dot - pos_self; 	
			angular_direction_other = torque_dot - pos_other;
		}

		float torque_self = Vec::Cross(angular_direction_self, impulse / 100.f);
		float torque_other = Vec::Cross(angular_direction_other, Vec::Reverse(impulse) / 100.f);

		rigidbody_self->ApplyAngular(torque_self / rigidbody_self->GetMass());
		rigidbody_other->ApplyAngular(torque_other / rigidbody_other->GetMass());
	}
}

ContactInfo GetCollisionPart(uint32_t self_entity_id, uint32_t other_entity_id)
{
	auto coll_self = SceneMgr::GetComponent<ColliderComponent>(self_entity_id);
	auto coll_other = SceneMgr::GetComponent<ColliderComponent>(other_entity_id);

	ContactInfo contact_info{};

	if (coll_self && coll_other) {
		auto obb_self = coll_self->GetOBB();
		auto obb_other = coll_other->GetOBB();

		Vec2 self_pos = SceneMgr::GetComponent<TransformComponent>(self_entity_id)->GetPos();
		Vec2 other_pos = SceneMgr::GetComponent<TransformComponent>(other_entity_id)->GetPos();

		auto self_left_bot = self_pos - obb_self.width_half - obb_self.height_half;
		auto self_left_top = self_pos - obb_self.width_half + obb_self.height_half;
		auto self_right_bot = self_pos + obb_self.width_half - obb_self.height_half;
		auto self_right_top = self_pos + obb_self.width_half + obb_self.height_half;

		auto other_left_bot = other_pos - obb_other.width_half - obb_other.height_half;
		auto other_left_top = other_pos - obb_other.width_half + obb_other.height_half;
		auto other_right_bot = other_pos + obb_other.width_half - obb_other.height_half;
		auto other_right_top = other_pos + obb_other.width_half + obb_other.height_half;

		std::vector<Vec2> self_vertexs{ self_left_bot, self_left_top, self_right_top, self_right_bot };
		std::vector<Vec2> other_vertexs{ other_left_bot, other_left_top, other_right_top, other_right_bot };

		float distn_min = std::numeric_limits<float>::max();
		Vec2 contact_point{};
		float sum_distn_center{};
		
		for (const auto& v : self_vertexs){
			auto length = Vec::LengthSquare(other_pos - v);
			if ( length < distn_min){
				distn_min = length;
				contact_point = v;
			}
		}

		sum_distn_center = Vec::LengthSquare(contact_point - other_pos) 
						+ Vec::LengthSquare(contact_point - self_pos);

		Vec2 temp;
		distn_min = std::numeric_limits<float>::max();
		for (const auto& v : other_vertexs){
			auto length = Vec::LengthSquare(self_pos - v);
			if ( length < distn_min){
				distn_min = length;
				temp = v;
			}
		}
		
		// if collision part is edge
		auto vec_edge_self = Edge::CreateEdge(self_vertexs);
		auto vec_edge_other = Edge::CreateEdge(other_vertexs);
		// vec_edge_self에 합침
		vec_edge_self.insert(vec_edge_self.end(), vec_edge_other.begin(), vec_edge_other.end());
		Vec2 contact_edge {contact_point-temp};
		
		bool coll_by_side{true};

		for ( auto edge : vec_edge_self){
			auto result = Vec::Dot(edge.end-edge.start,contact_edge);
			if ( (result > 0)  && (Vec::Normalize(edge.end-edge.start) != Vec::Normalize(contact_edge))) {
				// 예각이다.
				coll_by_side = false;
				break;
			}
		}
		if ( coll_by_side ) {
			contact_info.coll_by_side = true;
			contact_info.contact_point_self = contact_point;
			contact_info.contact_point_other = temp;
			return contact_info;
		}

		contact_info.coll_by_side = false;
		
		if ( sum_distn_center < Vec::LengthSquare(temp - other_pos) 
			+ Vec::LengthSquare(temp - self_pos)){
			contact_info.contact_point_self = contact_point;
			return contact_info;
		}

		contact_info.contact_point_self = temp;
		return contact_info;
	}

	contact_info.contact_point_self = Vec2(0, 0);
	return contact_info;
}
