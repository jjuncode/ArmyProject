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

	// 최대 속도 제한
	if (Vec::Length(m_velocity) > Vec::Length(m_velocity_max)) {
		m_velocity = Vec::Normalize(m_velocity) * Vec::Length(m_velocity_max);
	}

	// Get rid of too low velo & angular
	constexpr float VELOCITY_EPSILON = 0.1f;
	constexpr float ANGULAR_EPSILON = 0.05f;

	if ( abs(m_velocity.x )< VELOCITY_EPSILON ) {
		m_velocity.x = 0;
	}
	if (abs( m_velocity.y )< VELOCITY_EPSILON){
		m_velocity.y = 0;
	}

	if (std::abs(m_velo_angular) < ANGULAR_EPSILON){
		m_velo_angular = 0;
	}

	// Apply Angular
	auto rotate_value = m_velo_angular * dt;
	transform->AddRotate(rotate_value);

	m_velo_angular *= 0.9f;

	transform->AddPos(m_velocity * dt);
} 

void Physic::ProcessPhysicCollision(uint32_t self_entity_id, uint32_t other_entity_id, MTV _mtv, float dt)
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
			j = -(1 + elastic) * velo_extract / ((1.0f / rigidbody_self->GetMass()));
		}
		else
			j = -(1 + elastic) * velo_extract / ((1.0f / rigidbody_self->GetMass()) + (1.0f / rigidbody_other->GetMass()));
		
		// Get rid of too low j
		if ( j < 100){
			j =0;
		}

		// Get Impulse
		Vec2 impulse = direction * abs(j) / mass_self;
	
		// Apply Torque
		{
			auto obb = coll_self->GetOBB();
			float width_square = abs(Vec::LengthSquare(obb.width_half)) ;
			float height_square = abs(Vec::LengthSquare(obb.height_half));

			float inertia = (1.0f / 12.0f) * mass_self * (width_square + height_square);
			auto ground_vec = Physic::GetGroundVec(obb.width_half, obb.height_half, vec_contact[0], direction);
		
			auto pos_self = transform_self->GetPos();
			auto pos_other = transform_other->GetPos();
			auto pos_center = (pos_self + pos_other) / 2;

			Vec2 angular_direction_self{};
			float additional_torque{1};

			if (vec_contact.size() == 1 ){
				// coll by Dot 
				angular_direction_self = vec_contact[0] - pos_self; // Dot 충돌이므로 다 self로 해도 상관없다.

				// 점충돌일 때 좀 크게해줌
				additional_torque = Vec::Dot(Vec::Normalize(angular_direction_self), Vec::Normal(direction));
				additional_torque = (abs(additional_torque)+1 )*2.f;
			}
			else{
				// coll by side
				// vertical collision
				auto distn_1 = Vec::LengthSquare(vec_contact[0] - pos_self);
				auto distn_2 = Vec::LengthSquare(vec_contact[1] - pos_self);

				auto distn_dot = Vec::LengthSquare(vec_contact[0] - vec_contact[1]);
				auto dot_proj = Vec::Projection(ground_vec.first, vec_contact[0]);
				auto pos_proj = Vec::Projection(ground_vec.first, pos_self);
				auto radius = Vec::LengthSquare(dot_proj- pos_proj);

				if ( radius < distn_dot){
					// bigger than radius 
					// can't be rotate
					angular_direction_self = Vec2(0,0);
				}
 				else if ( distn_1 == distn_2 ){
					// Don't Rotate
					angular_direction_self = Vec2(0,0);
				}
				else{
					Vec2 torque_dot{};
					// 내 중심에서 좀더 먼곳으로 
					if ( distn_1 > distn_2 ) 
						torque_dot = vec_contact[0];
					else
						torque_dot = vec_contact[1];

					angular_direction_self = torque_dot - pos_self;
				}
			}

			// Get Torque
			float torque_self = Vec::Cross(angular_direction_self, impulse) * additional_torque;
			torque_self /= inertia;
			
			// Apply Torque
			rigidbody_self->ApplyAngular(torque_self);
			auto angular = rigidbody_self->GetAngularVelocity() * 0.9f;

			// 안착된 상태 판정하기
			if (vec_contact.size() == 1){
				// Only in Dot Collision 
				//torque_self *= ground_vec.second;

				if (ground_vec.second < 5.f && rigidbody_self->GetVelocity().x ==0 ){
					// 지면과의 각도가 1도 이하라면 지면에 부착 
					// x방향으로의 속도가 없으면 안착한 것임
					// 멈춰버렷
					angular = 0;
					torque_self = 0; 

					auto cross = Vec::Cross(angular_direction_self, direction);
					float rotate_drc{ cross / abs(cross)};
					transform_self->AddRotate(Vec::GetRadian(ground_vec.second) * rotate_drc);
				
					rigidbody_self->AddForce(Vec::Reverse(rigidbody_self->GetGravity())*mass_self);
				}
			}
		
			rigidbody_self->SetAngularVelocity(angular);
		}

		// Apply Fric & impulse
		rigidbody_self->SetVelocity(rigidbody_self->GetVelocity() * fric);
		rigidbody_self->ApplyImpulse(impulse);
	}
}

std::pair<Vec2,float> Physic::GetGroundVec(Vec2 _width, Vec2 _height, Vec2 _contact_dot, Vec2 _mtv_direction)
{
	auto width_degree = Vec::GetDegree(_mtv_direction, _width);
	auto height_degree = Vec::GetDegree(_mtv_direction, _height);

	// bigger is ground vec 
	std::pair<Vec2,float> ground_vec{};
	if ( width_degree > 90.f ) width_degree = 180.f - width_degree;
	if ( height_degree > 90.f ) height_degree = 180.f - height_degree;

	if ( width_degree > height_degree ){
		ground_vec.first = Vec::Normalize(_width);
		// degree by ground ( 지면과의 각도 값 ) 
		ground_vec.second =abs( 90.f - width_degree);
	}
	else{
		ground_vec.first = Vec::Normalize(_height);
		// degree by ground ( 지면과의 각도 값 ) 
		ground_vec.second =abs(90.f - height_degree);
	}

	return ground_vec; 
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
