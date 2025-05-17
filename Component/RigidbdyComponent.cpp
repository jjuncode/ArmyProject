#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

#include "../Mgr/SceneMgr.h"
#include <algorithm>


void Rigidbody::Update(float dt)
{
    auto transform = SceneMgr::GetComponent<TransformComponent>(GetOwnerID());
	auto pos = transform->GetPos();

	if (IsFixed())
		return;

	// Apply gravity
	Vec2 force_gravity = m_gravity * m_mass;
	m_force += force_gravity;

	// a = f / m;
	m_accel = m_force / m_mass;

	// v = v0 + a * t
	m_velocity += m_accel * dt;
	m_velocity += m_acc_impulse;
	m_acc_impulse = Vec2(0,0);
	m_force = Vec2(0,0);

	// Apply Angular
	auto rotate_value = m_velo_angular * dt;
	transform->AddRotate(rotate_value);

	float angular_damp = 0.2f;	// 감쇠 비율 20%
	m_velo_angular *= (1 - angular_damp);

	// 최대 속도 제한
	if (Vec::Length(m_velocity) > Vec::Length(m_velocity_max)) {
		m_velocity = Vec::Normalize(m_velocity) * Vec::Length(m_velocity_max);
	}

	// Get rid of too low velo & angular
	constexpr float VELOCITY_EPSILON = 1.f;
	constexpr float ANGULAR_EPSILON = 0.1f;

	if (Vec::LengthSquare(m_velocity) < VELOCITY_EPSILON)
	{
		m_velocity = Vec2(0.0f, 0.0f);
	}

	if (std::abs(m_velo_angular) < ANGULAR_EPSILON)
	{
		m_velo_angular = 0;
	}

	// // on ground 
	// if ( pos.y <= 0 ){
	// 	m_velocity.x *= m_fric;
	// 	m_velo_angular *= 0.5f;

	// 	transform->SetPos(Vec2(pos.x, 0));
	// 	if ( m_velocity.y < 0 )
	// 		m_velocity.y = 0;
	// }

	transform->AddPos(m_velocity * dt);
} 

void ProcessPhysicCollision(uint32_t self_entity_id, uint32_t other_entity_id, MTV _mtv, float dt)
{
	auto transform_self = SceneMgr::GetComponent<TransformComponent>(self_entity_id);
	auto rigidbody_self = SceneMgr::GetComponent<Rigidbody>(self_entity_id);
	auto coll_self = SceneMgr::GetComponent<ColliderComponent>(self_entity_id);

	if ( rigidbody_self->IsFixed() )
		return;
	
    auto transform_other = SceneMgr::GetComponent<TransformComponent>(other_entity_id);
    auto rigidbody_other = SceneMgr::GetComponent<Rigidbody>(other_entity_id);

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

		auto move_value = direction *_mtv.length * move_ratio;

		// Get Contact point
		auto vec_contact = GetCollisionCandidate(self_entity_id, other_entity_id, direction);
		for(auto& dot : vec_contact)
			dot += move_value;

		// Apply MTV
		transform_self->AddPos(move_value);

		// Get Elasticity
		Vec2 relative_velo = rigidbody_self->GetVelocity() - rigidbody_other->GetVelocity();
		float elastic = (rigidbody_self->GetElastic() + rigidbody_other->GetElastic()) / 2;

		// Get Friction
		float fric = (rigidbody_self->GetFric() + rigidbody_other->GetFric()) / 2;
		
		// Get Impulse
		float velo_extract = Vec::Dot(relative_velo, direction);
		float j{};
		if ( rigidbody_other->IsFixed() ){
			j = -(1 + elastic) * velo_extract / ((2.0f / rigidbody_self->GetMass()));
		}
		else
			j = -(1 + elastic) * velo_extract / ((1.0f / rigidbody_self->GetMass()) + (1.0f / rigidbody_other->GetMass()));
		
		// Get rid of too low j
		if ( j < 100){
			j =0;
		}
		Vec2 impulse = direction * abs(j) / mass_self;

		auto pos_self = transform_self->GetPos();
		auto pos_other = transform_other->GetPos();
		auto pos_center = (pos_self + pos_other) / 2;

		Vec2 angular_direction_self{};

		if (vec_contact.size() == 1 ){
			// coll by Dot 
			angular_direction_self = vec_contact[0] - pos_self; // Dot 충돌이므로 다 self로 해도 상관없다.
		}
		else{
			// coll by side
			// vertical collision
			auto distn_1 = Vec::LengthSquare(vec_contact[0] - pos_self);
			auto distn_2 = Vec::LengthSquare(vec_contact[1] - pos_self);
 			if ( distn_1 == distn_2 ){
				// Don't Rotate
				angular_direction_self = Vec2(0,0);
			}
			else{
				Vec2 torque_dot{};
				torque_dot = (vec_contact[0] + vec_contact[1]) / 2;

				angular_direction_self = torque_dot - pos_self;
			}
		}

		auto obb = coll_self->GetOBB();

		float width_square = abs(Vec::LengthSquare(obb.width_half)) ;
		float height_square = abs(Vec::LengthSquare(obb.height_half));

		float inertia = (1.0f / 12.0f) * mass_self * (width_square + height_square);

		// Get Torque
		float torque_self = Vec::Cross(angular_direction_self, impulse);
		torque_self /= inertia;

		// Apply Fric & impulse & Torque
		rigidbody_self->SetVelocity(rigidbody_self->GetVelocity() * fric);
		rigidbody_self->ApplyImpulse(impulse);
		rigidbody_self->SetAngularVelocity(rigidbody_self->GetAngularVelocity() * fric);
		rigidbody_self->ApplyAngular(torque_self);
	}
}

std::vector<Vec2> GetCollisionCandidate(uint32_t self_entity_id, uint32_t other_entity_id, Vec2 _mtv_vec)
{
	auto coll_self = SceneMgr::GetComponent<ColliderComponent>(self_entity_id);
	auto coll_other = SceneMgr::GetComponent<ColliderComponent>(other_entity_id);

	Vec2 pos_self = SceneMgr::GetComponent<TransformComponent>(self_entity_id)->GetPos();
	Vec2 pos_other = SceneMgr::GetComponent<TransformComponent>(other_entity_id)->GetPos();
	
	bool coll_by_side{true};
	
	auto obb_self = coll_self->GetOBB();
	auto obb_other = coll_other->GetOBB();
	auto self_left_bot = pos_self - obb_self.width_half - obb_self.height_half;
	auto self_left_top = pos_self - obb_self.width_half + obb_self.height_half;
	auto self_right_bot = pos_self + obb_self.width_half - obb_self.height_half;
	auto self_right_top = pos_self + obb_self.width_half + obb_self.height_half;
	auto other_left_bot = pos_other - obb_other.width_half - obb_other.height_half;
	auto other_left_top = pos_other - obb_other.width_half + obb_other.height_half;
	auto other_right_bot = pos_other + obb_other.width_half - obb_other.height_half;
	auto other_right_top = pos_other + obb_other.width_half + obb_other.height_half;
	
	std::vector<Vec2> self_vertexs{ self_left_bot, self_left_top, self_right_top, self_right_bot };
	std::vector<Vec2> other_vertexs{ other_left_bot, other_left_top, other_right_top, other_right_bot };
	
	auto pos_self_proj = Vec::Projection(_mtv_vec, pos_self);
	auto pos_other_proj = Vec::Projection(_mtv_vec, pos_other);

	std::vector<Vec2> vec_contact{};
	std::vector<Vec2> self_contact_candidates;
	std::vector<Vec2> other_contact_candidates;

	// 충돌 후보군 찾기 1 
	{
		Vec2 self_dot_min_1{}, self_dot_min_2{};
		float distn_min_1{std::numeric_limits<float>::max()};
		float distn_min_2{std::numeric_limits<float>::max()};

		for (auto &dot : self_vertexs){
			auto proj_dot = Vec::Projection(_mtv_vec, dot);
			auto distn = Vec::LengthSquare(proj_dot - pos_other_proj);

			if (distn < distn_min_1){
				// 현재가 가장 짧은 거리라면, 두 번째 짧은 거리를 이전에 가장 짧았던 거리로 설정
				distn_min_2 = distn_min_1;
				self_dot_min_2 = self_dot_min_1;

				// 가장 짧은 거리 갱신
				distn_min_1 = distn;
				self_dot_min_1 = dot;
			}
			else if (distn < distn_min_2){
				// 두 번째로 짧은 거리 갱신
				distn_min_2 = distn;
				self_dot_min_2 = dot;
			}
		}

		self_contact_candidates.emplace_back(self_dot_min_1);
		self_contact_candidates.emplace_back(self_dot_min_2);
		
		vec_contact.emplace_back(self_dot_min_1);
		vec_contact.emplace_back(self_dot_min_2);
	}
	// 충돌 후보군 찾기 2
	{
		Vec2 other_dot_min_1{}, other_dot_min_2{};
		
		float distn_min_1{std::numeric_limits<float>::max()};
		float distn_min_2{std::numeric_limits<float>::max()};
		for (auto &dot : other_vertexs){
			auto dot_proj = Vec::Projection(_mtv_vec, dot);
			auto distn = Vec::LengthSquare(dot_proj - pos_self_proj);
			if (distn < distn_min_1){
				// 현재가 가장 짧은 거리라면, 두 번째 짧은 거리를 이전에 가장 짧았던 거리로 설정
				distn_min_2 = distn_min_1;
				other_dot_min_2 = other_dot_min_1;
				// 가장 짧은 거리 갱신
				distn_min_1 = distn;
				other_dot_min_1 = dot;
			}
			else if (distn < distn_min_2){
				// 두 번째로 짧은 거리 갱신
				distn_min_2 = distn;
				other_dot_min_2 = dot;
			}
		}
		
		other_contact_candidates.emplace_back(other_dot_min_1);
		other_contact_candidates.emplace_back(other_dot_min_2);
		
		vec_contact.emplace_back(other_dot_min_1);
		vec_contact.emplace_back(other_dot_min_2);
	}
	

	// Is Coll by Side?
	Vec2 v1 = self_contact_candidates[0] - self_contact_candidates[1];
	Vec2 v2 = other_contact_candidates[0] - other_contact_candidates[1];
	if (Vec::Dot(v1, v2) == 0 || Vec::Cross(v1, v2) == 0) {
		coll_by_side = true;
	} else {
		coll_by_side = false;
	}

	if ( !coll_by_side ) {
		// contact by dot 
		// Narrow to two candidates
		std::vector<Vec2> vec_proj;
		for ( auto& dot : vec_contact){
			auto dot_proj = Vec::Projection(_mtv_vec, dot);
			vec_proj.emplace_back(dot_proj);
		}

		float distn_min{std::numeric_limits<float>::max()};
		uint32_t dot_candidate_idx_1{};
		uint32_t dot_candidate_idx_2{};
		
		for ( int i=0; i<vec_proj.size(); ++i ) {
			for ( int j=i+1; j<vec_proj.size(); ++j){
				auto distn = Vec::LengthSquare(vec_proj[i] - vec_proj[j]);
				
				if ( distn == 0 ) continue;

				if ( distn < distn_min ){
					// 현재가 가장 짧은 거리라면, 두 번째 짧은 거리를 이전에 가장 짧았던 거리로 설정
					dot_candidate_idx_1 = i;
					dot_candidate_idx_2 = j;
					distn_min = distn;
				}
			}
		}

		// Find Closest Dot
		auto distn_1 = Vec::LengthSquare(vec_contact[dot_candidate_idx_1] - pos_self)
			+ Vec::LengthSquare(vec_contact[dot_candidate_idx_1] - pos_other);
		auto distn_2 = Vec::LengthSquare(vec_contact[dot_candidate_idx_2] - pos_self)
			+ Vec::LengthSquare(vec_contact[dot_candidate_idx_2] - pos_other);
		if ( distn_1 < distn_2 ){
			vec_contact.clear();
			vec_contact.emplace_back(vec_contact[dot_candidate_idx_1]);
		}
		else{
			vec_contact.clear();
			vec_contact.emplace_back(vec_contact[dot_candidate_idx_2]);
		}

	}
	else {
		// contact by side
		float distn_min_1{std::numeric_limits<float>::max()};
		float distn_min_2{std::numeric_limits<float>::max()};
		Vec2 dot_min_1{};
		Vec2 dot_min_2{};
		for (auto &dot : vec_contact){
			auto distn = Vec::LengthSquare(dot - pos_self) + Vec::LengthSquare(dot - pos_other);
			if (distn < distn_min_1){
				// 현재가 가장 짧은 거리라면, 두 번째 짧은 거리를 이전에 가장 짧았던 거리로 설정
				distn_min_2 = distn_min_1;
				dot_min_2 = dot_min_1;
				// 가장 짧은 거리 갱신
				distn_min_1 = distn;
				dot_min_1 = dot;
			}
			else if (distn < distn_min_2){
				// 두 번째로 짧은 거리 갱신
				distn_min_2 = distn;
				dot_min_2 = dot;
			}
		}
		vec_contact.clear();
		vec_contact.emplace_back(dot_min_1);
		vec_contact.emplace_back(dot_min_2);
	}

	return vec_contact;
}
